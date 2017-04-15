/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "internal/abstract-shell-view-private.hpp"

#include <skland/core/assert.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/gui/abstract-shell-frame.hpp>

#include <skland/stock/theme.hpp>

#include "internal/abstract-view-private.hpp"
#include "internal/display-registry.hpp"
#include "internal/abstract-event-handler-mouse-task-iterator.hpp"

namespace skland {

AbstractShellView::AbstractShellView(const char *title, AbstractShellView *parent)
    : AbstractShellView(400, 300, title, parent) {
}

AbstractShellView::AbstractShellView(int width,
                                     int height,
                                     const char *title,
                                     AbstractShellView *parent)
    : AbstractEventHandler() {
  p_.reset(new Private(this));
  p_->size.width = width;
  p_->size.height = height;
  p_->last_size = p_->size;
  p_->parent = parent;

  if (title) p_->title = title;

  if (nullptr == p_->parent) {
    p_->shell_surface = Surface::Shell::Toplevel::Create(this, Theme::shadow_margin());
    Surface::Shell::Get(p_->shell_surface)->configure().Set(this, &AbstractShellView::OnXdgSurfaceConfigure);
    Surface::Shell::Toplevel *top_level_role = Surface::Shell::Toplevel::Get(p_->shell_surface);
    top_level_role->configure().Set(this, &AbstractShellView::OnXdgToplevelConfigure);
    top_level_role->close().Set(this, &AbstractShellView::OnXdgToplevelClose);
    top_level_role->SetTitle(title);
  } else {
    // TODO: create popup shell surface
  }

  int x = 0, y = 0;  // The input region
  x += Theme::shadow_margin().left - AbstractShellFrame::kResizingMargin.left;
  y += Theme::shadow_margin().top - AbstractShellFrame::kResizingMargin.top;
  width += AbstractShellFrame::kResizingMargin.lr();
  height += AbstractShellFrame::kResizingMargin.tb();

  wayland::Region input_region;
  input_region.Setup(Display::Registry().wl_compositor());
  input_region.Add(x, y, width, height);
  p_->shell_surface->SetInputRegion(input_region);
}

AbstractShellView::~AbstractShellView() {
  delete p_->shell_surface;
}

void AbstractShellView::SetTitle(const char *title) {
  p_->title = title;
  if (nullptr == p_->parent) {
    Surface::Shell::Toplevel::Get(p_->shell_surface)->SetTitle(title);
  }
}

void AbstractShellView::SetAppId(const char *app_id) {
  p_->app_id = app_id;
  if (nullptr == p_->parent) {
    Surface::Shell::Toplevel::Get(p_->shell_surface)->SetAppId(app_id);
  }
}

void AbstractShellView::Show() {
  if (!IsShown()) {
    p_->shell_surface->Commit();
  }
}

void AbstractShellView::Close(SLOT) {
  if (Surface::GetShellSurfaceCount() == 1) {
    Application::Exit();
  }

  // TODO: use a close task if there's more than 1 windows in an application

  // windows will be deleted when application exits, uncomment this line
  // sometimes cause segfault when close button is clicked:
  //  delete this;
}

void AbstractShellView::Minimize(SLOT) {
  Surface::Shell::Toplevel *toplevel = Surface::Shell::Toplevel::Get(p_->shell_surface);
  if (nullptr == toplevel) return;

  Bit::Set<int>(p_->flags, Private::kFlagMaskMinimized);
  toplevel->SetMinimized();
  _ASSERT(IsMinimized());
}

void AbstractShellView::ToggleMaximize(SLOT) {
  if (IsFullscreen()) return;

  Surface::Shell::Toplevel *toplevel = Surface::Shell::Toplevel::Get(p_->shell_surface);
  if (nullptr == toplevel) return;

  if (IsMaximized()) {
    toplevel->UnsetMaximized();
  } else {
    toplevel->SetMaximized();
  }
}

void AbstractShellView::ToggleFullscreen(SLOT slot) {
  Surface::Shell::Toplevel *toplevel = Surface::Shell::Toplevel::Get(p_->shell_surface);
  if (nullptr == toplevel) return;

  if (p_->output) {
    if (IsFullscreen())
      toplevel->UnsetFullscreen(*p_->output);
    else
      toplevel->SetFullscreen(*p_->output);
  }
}

const std::string &AbstractShellView::GetTitle() const {
  return p_->title;
}

Size AbstractShellView::GetMinimalSize() const {
  int w = 160, h = 120;
//  Rect client = GetClientGeometry();
//  switch (window_frame_->title_bar_position()) {
//    case AbstractWindowFrame::kTitleBarLeft:
//    case AbstractWindowFrame::kTitleBarRight: {
//      w = window_frame_->title_bar_size() + window_frame_->border();
//      w += 120;
//      break;
//    }
//    case AbstractWindowFrame::kTitleBarBottom:
//    case AbstractWindowFrame::kTitleBarTop:
//    default: {
//      h = window_frame_->title_bar_size() + window_frame_->border();
//      h += 90;
//      break;
//    }
//  }
  return Size(w, h);
}

Size AbstractShellView::GetPreferredSize() const {
  return Size(640, 480);
}

Size AbstractShellView::GetMaximalSize() const {
  return Size(65536, 65536);
}

bool AbstractShellView::IsFullscreen() const {
  return 0 != (p_->flags & Private::kFlagMaskFullscreen);
}

bool AbstractShellView::IsMaximized() const {
  return 0 != (p_->flags & Private::kFlagMaskMaximized);
}

bool AbstractShellView::IsMinimized() const {
  return 0 != (p_->flags & Private::kFlagMaskMinimized);
}

bool AbstractShellView::IsFocused() const {
  return 0 != (p_->flags & Private::kFlagMaskFocused);
}

bool AbstractShellView::IsResizing() const {
  return 0 != (p_->flags & Private::kFlagMaskResizing);
}

bool AbstractShellView::IsShown() const {
  return 0 != (p_->flags & Private::kFlagMaskShown);
}

int AbstractShellView::GetWidth() const {
  return p_->size.width;
}

int AbstractShellView::GetHeight() const {
  return p_->size.height;
}

void AbstractShellView::AttachView(AbstractView *view) {
  if (view->p_->shell == this) {
    _ASSERT(nullptr == view->p_->parent);
    return;
  }

  if (view->p_->parent) {
    _ASSERT(nullptr == view->p_->shell);
    view->p_->parent->RemoveChild(view);
    _ASSERT(nullptr == view->p_->parent);
    _ASSERT(nullptr == view->p_->previous);
    _ASSERT(nullptr == view->p_->next);
  } else if (view->p_->shell) {
    _ASSERT(nullptr == view->p_->parent);
    view->p_->shell->DetachView(view);
  }

  view->p_->shell = this;

  OnViewAttached(view);
  if (view->p_->shell == this)
    view->OnAttachedToShellView();
}

void AbstractShellView::DetachView(AbstractView *view) {
  if (view->p_->shell != this) return;

  _ASSERT(nullptr == view->p_->parent);
  view->p_->shell = nullptr;

  OnViewDetached(view);
  if (view->p_->shell != this)
    view->OnDetachedFromShellView(this);
}

void AbstractShellView::OnMouseEnter(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnMouseLeave() {
  // override in sub class
}

void AbstractShellView::OnMouseMove(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnMouseButton(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnKeyboardKey(KeyEvent *event) {
  // override in sub class
}

void AbstractShellView::OnUpdate(AbstractView *view) {
  // override in sub class
}

Surface *AbstractShellView::GetSurface(const AbstractView * /* view */) const {
  return p_->shell_surface;
}

void AbstractShellView::OnEnterOutput(const Output *output) {
  p_->output = output;
}

void AbstractShellView::OnLeaveOutput(const Output *output) {
  if (p_->output != output) {
    fprintf(stderr, "Warning! Registered output object does not match!\n");
  }

  p_->output = nullptr;
}

void AbstractShellView::OnDraw(const Context *context) {
  // override in sub class
}

void AbstractShellView::OnMaximized(bool maximized) {

}

void AbstractShellView::OnFullscreen(bool) {

}

void AbstractShellView::OnFocus(bool focus) {
  // override in sub class
}

void AbstractShellView::OnViewAttached(AbstractView *view) {

}

void AbstractShellView::OnViewDetached(AbstractView *view) {

}

void AbstractShellView::MoveWithMouse(MouseEvent *event) const {
  Surface::Shell::Toplevel::Get(p_->shell_surface)->Move(event->GetSeat(), event->GetSerial());
}

void AbstractShellView::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  Surface::Shell::Toplevel::Get(p_->shell_surface)->Resize(event->GetSeat(), event->GetSerial(), edges);
}

Surface *AbstractShellView::GetShellSurface() const {
  return p_->shell_surface;
}

void AbstractShellView::Damage(AbstractShellView *shell_view,
                               int surface_x, int surface_y,
                               int width, int height) {
  shell_view->p_->is_damaged = true;
  shell_view->p_->damaged_region.l = surface_x;
  shell_view->p_->damaged_region.t = surface_y;
  shell_view->p_->damaged_region.Resize(width, height);
}

void AbstractShellView::Damage(AbstractView *view,
                               int surface_x, int surface_y,
                               int width, int height) {
  view->p_->is_damaged = true;
  view->p_->damaged_region.l = surface_x;
  view->p_->damaged_region.t = surface_y;
  view->p_->damaged_region.Resize(width, height);
}

void AbstractShellView::RecursiveUpdate(AbstractView *view) {
  view->RecursiveUpdate();
}

void AbstractShellView::OnXdgSurfaceConfigure(uint32_t serial) {
  Surface::Shell *shell_surface = Surface::Shell::Get(p_->shell_surface);
  shell_surface->AckConfigure(serial);

  if (!IsShown()) {
    Bit::Set<int>(p_->flags, Private::kFlagMaskShown);
    shell_surface->ResizeWindow(p_->size.width, p_->size.height);
    OnShown();
  }
}

void AbstractShellView::OnXdgToplevelConfigure(int width, int height, int states) {
  using wayland::XdgToplevel;

  bool maximized = (0 != (states & XdgToplevel::kStateMaskMaximized));
  bool fullscreen = (0 != (states & XdgToplevel::kStateMaskFullscreen));
  bool resizing = (0 != (states & XdgToplevel::kStateMaskResizing));
  bool focus = (0 != (states & XdgToplevel::kStateMaskActivated));

  bool do_resize = true;

  if (width > 0 && height > 0) {
    Size min = this->GetMinimalSize();
    Size max = this->GetMaximalSize();
    _ASSERT(min.width < max.width && min.height < max.height);

    width = clamp(width, min.width, max.width);
    height = clamp(height, min.height, max.height);
    if (width == p_->size.width && height == p_->size.height) {
      do_resize = false;
    } else {
      p_->size.width = width;
      p_->size.height = height;
    }
  }

  if (maximized != IsMaximized()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskMaximized);
    OnMaximized(maximized);
  }

  if (fullscreen != IsFullscreen()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskFullscreen);
    OnFullscreen(fullscreen);
  }

  if (do_resize) {
    Surface::Shell::Get(p_->shell_surface)->ResizeWindow(width, height);  // Call xdg surface api
    OnSizeChange(p_->last_size, p_->size);
    p_->last_size = p_->size;

    // surface size is changed, reset the pointer position and enter/leave widgets
    DispatchMouseLeaveEvent();
  }

  if (focus != IsFocused()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskFocused);
    OnFocus(focus);
  }

  if (resizing != IsResizing()) {
    // TODO: no need to use this flag
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskResizing);
  }
}

void AbstractShellView::OnXdgToplevelClose() {
  Close();
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *view, MouseEvent *event) {
  Point cursor = event->GetWindowXY();
  MouseTaskIterator it(this);

  if (nullptr == it.next()) {
    _ASSERT(it.mouse_task()->event_handler == this);
    _ASSERT(nullptr == it.previous());
    if (view->Contain(cursor.x, cursor.y)) {
      view->OnMouseEnter(event);
      if (event->IsAccepted()) {
        it.PushBack(view);
        ++it;
        DispatchMouseEnterEvent(view, event, it);
      } else if (event->IsIgnored()) {
        DispatchMouseEnterEvent(view, event, it);
      }
    }
  } else {
    while (it.next()) ++it; // move to tail
    AbstractView *last = nullptr;
    EventTask *tail = nullptr;
    while (it.previous()) {
      tail = it.mouse_task();
      last = static_cast<AbstractView *>(tail->event_handler);
      if (last->Contain(cursor.x, cursor.y)) {
        break;
      }
      --it;
      tail->Unlink();
      last->OnMouseLeave();
      if (nullptr == it.previous()) break;
    }

    DispatchMouseEnterEvent(last, event, it);
  }
}

void AbstractShellView::DispatchMouseLeaveEvent() {
  MouseTaskIterator it(this);
  EventTask *task = nullptr;
  ++it;

  while (it) {
    task = it.mouse_task();
    ++it;
    task->Unlink();
    static_cast<AbstractView *>(task->event_handler)->OnMouseLeave();
  }
}

void AbstractShellView::DispatchMouseButtonEvent(MouseEvent *event) {
  MouseTaskIterator it(this);
  ++it;

  AbstractView *view = nullptr;
  while (it) {
    view = static_cast<AbstractView *>(it.mouse_task()->event_handler);
    view->OnMouseButton(event);
    if (event->IsRejected()) break;
    ++it;
  }
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *parent, MouseEvent *event, MouseTaskIterator &tail) {
  AbstractView *sub = parent->DispatchMouseEnterEvent(event);
  while (sub) {
    _ASSERT(sub != parent);
    sub->OnMouseEnter(event);
    if (event->IsAccepted()) {
      tail.PushBack(sub);
      ++tail;
      parent = sub;
      sub = parent->DispatchMouseEnterEvent(event);
    } else if (event->IsIgnored()) {
      parent = sub;
      sub = parent->DispatchMouseEnterEvent(event);
    } else {
      break;
    }
  }
}

}
