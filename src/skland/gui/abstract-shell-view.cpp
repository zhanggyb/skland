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

#include <skland/gui/application.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/gui/shell-surface.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>

#include <skland/gui/abstract-shell-frame.hpp>

#include <skland/stock/theme.hpp>

#include "internal/abstract-shell-frame-private.hpp"
#include "internal/abstract-view-private.hpp"
#include "internal/display-registry.hpp"
#include "internal/abstract-event-handler-mouse-task-iterator.hpp"

namespace skland {

AbstractShellView::AbstractShellView(const char *title, AbstractShellView *parent,
                                     AbstractShellFrame *frame)
    : AbstractShellView(400, 300, title, parent, frame) {
}

AbstractShellView::AbstractShellView(int width,
                                     int height,
                                     const char *title,
                                     AbstractShellView *parent,
                                     AbstractShellFrame *frame)
    : AbstractEventHandler() {
  p_.reset(new Private(this));
  p_->size.width = width;
  p_->size.height = height;
  p_->parent = parent;

  if (title) p_->title = title;

  if (nullptr == p_->parent) {
    p_->shell_surface = ToplevelShellSurface::Create(this, Theme::shadow_margin());
    ShellSurface::Get(p_->shell_surface)->configure().Set(this, &AbstractShellView::OnXdgSurfaceConfigure);
    ToplevelShellSurface *top_level_role = ToplevelShellSurface::Get(p_->shell_surface);
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

  SetShellFrame(frame);
}

AbstractShellView::~AbstractShellView() {
  SetClientView(nullptr); // this will destroy the client view

  if (p_->shell_frame) {
    p_->shell_frame->p_->shell_view = nullptr;
    Theme::DestroyWindowFrame(p_->shell_frame);
  }

  delete p_->shell_surface;
}

void AbstractShellView::SetTitle(const char *title) {
  p_->title = title;
  if (nullptr == p_->parent) {
    ToplevelShellSurface::Get(p_->shell_surface)->SetTitle(title);
  }
}

void AbstractShellView::SetAppId(const char *app_id) {
  p_->app_id = app_id;
  if (nullptr == p_->parent) {
    ToplevelShellSurface::Get(p_->shell_surface)->SetAppId(app_id);
  }
}

void AbstractShellView::SetShellFrame(AbstractShellFrame *shell_frame) {
  if (shell_frame == p_->shell_frame)
    return;

  if (p_->shell_frame) {
    DetachViewsOnFrame();
    p_->shell_frame->p_->shell_view = nullptr;
    delete p_->shell_frame; // This disconnect all action signals from the shell frame
  }

  p_->shell_frame = shell_frame;
  // TODO: check if there's original window using the window frame

  if (p_->shell_frame) {
    AttachViewsOnFrame();
    p_->shell_frame->action().Connect(this, &AbstractShellView::OnAction);

    p_->shell_frame->p_->shell_view = this;
    p_->shell_frame->OnSetup();
//    shell_frame_->OnResize(size_.width, size_.height);
  }

  OnUpdate(nullptr);
}

AbstractView *AbstractShellView::GetTitleView() const {
  if (p_->shell_frame) {
    AbstractView *view;
    for (int i = 0; i <= AbstractShellFrame::Position::kBottom; i++) {
      view = p_->shell_frame->GetViewAt(static_cast<AbstractShellFrame::Position>(i));
      if (view) return view;
    }
  }

  return nullptr;
}

void AbstractShellView::SetClientView(AbstractView *view) {
  if (p_->client_view == view) return;

  if (p_->client_view) {
    DetachView(p_->client_view);
    p_->client_view->destroyed().DisconnectAll(this, &AbstractShellView::OnClientViewDestroyed);
    p_->client_view->Destroy();
  }

  p_->client_view = view;

  if (p_->client_view) {
    AttachView(p_->client_view);
    p_->client_view->destroyed().Connect(this, &AbstractShellView::OnClientViewDestroyed);
    SetContentViewGeometry();
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

void AbstractShellView::Maximize(SLOT) {

}

void AbstractShellView::Minimize(SLOT) {

}

const std::string &AbstractShellView::GetTitle() const {
  return p_->title;
}

Size AbstractShellView::GetMinimalSize() const {
  if (IsFrameless()) return Size(100, 100);

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

int AbstractShellView::GetMouseLocation(const MouseEvent *event) const {
  if (nullptr == p_->shell_frame) {
    return kInterior;
  }

  return p_->shell_frame->GetMouseLocation(event);
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

bool AbstractShellView::IsFrameless() const {
  return nullptr == p_->shell_frame;
}

bool AbstractShellView::IsShown() const {
  return 0 != (p_->flags & Private::kFlagMaskShown);
}

const Size &AbstractShellView::GetSize() const {
  return p_->size;
}

AbstractView *AbstractShellView::GetClientView() const {
  return p_->client_view;
}

void AbstractShellView::AttachView(AbstractView *view) {
  if (view->p_->shell == this) {
    DBG_ASSERT(nullptr == view->p_->parent);
    return;
  }

  if (view->p_->parent) {
    DBG_ASSERT(nullptr == view->p_->shell);
    view->p_->parent->RemoveChild(view);
    DBG_ASSERT(nullptr == view->p_->parent);
    DBG_ASSERT(nullptr == view->p_->previous);
    DBG_ASSERT(nullptr == view->p_->next);
  } else if (view->p_->shell) {
    DBG_ASSERT(nullptr == view->p_->parent);
    view->p_->shell->DetachView(view);
  }

  view->p_->shell = this;

  OnViewAttached(view);
  if (view->p_->shell == this)
    view->OnAttachedToShellView();
}

void AbstractShellView::DetachView(AbstractView *view) {
  if (view->p_->shell != this) return;

  DBG_ASSERT(nullptr == view->p_->parent);
  view->p_->shell = nullptr;

  OnViewDetached(view);
  if (view->p_->shell != this)
    view->OnDetachedFromShellView(this);
}

Rect AbstractShellView::GetClientGeometry(int width, int height) const {
  if (nullptr == p_->shell_frame) {
    return Rect::FromXYWH(0.f, 0.f, width, height);
  }

  return p_->shell_frame->GetClientGeometry(width, height);
}

void AbstractShellView::OnMouseEnter(MouseEvent *event) {
  if (nullptr == p_->shell_frame) {
    return;
  }

  AbstractView *view = nullptr;
  int location = p_->shell_frame->GetMouseLocation(event);

  switch (location) {
    case kResizeTop: {
      event->SetCursor(Display::cursor(kCursorTop));
      break;
    }
    case kResizeBottom: {
      event->SetCursor(Display::cursor(kCursorBottom));
      break;
    }
    case kResizeLeft: {
      event->SetCursor(Display::cursor(kCursorLeft));
      break;
    }
    case kResizeRight: {
      event->SetCursor(Display::cursor(kCursorRight));
      break;
    }
    case kResizeTopLeft: {
      event->SetCursor(Display::cursor(kCursorTopLeft));
      break;
    }
    case kResizeTopRight: {
      event->SetCursor(Display::cursor(kCursorTopRight));
      break;
    }
    case kResizeBottomLeft: {
      event->SetCursor(Display::cursor(kCursorBottomLeft));
      break;
    }
    case kResizeBottomRight: {
      event->SetCursor(Display::cursor(kCursorBottomRight));
      break;
    }
    case kLeftSide: {
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kLeft);
      break;
    }
    case kTopSide: {
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kTop);
      break;
    }
    case kRightSide: {
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kRight);
      break;
    }
    case kBottomSide: {
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kBottom);
      break;
    }
    case kClientArea: {
      view = p_->client_view;
      break;
    }
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }

  if (view) {
    MouseTaskIterator it(this);
    while (it.next()) ++it; // move to tail
    DBG_ASSERT(nullptr == it.next());

    Point cursor = event->GetWindowXY();
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
  }
}

void AbstractShellView::OnMouseLeave() {
  ClearMouseTasks();
}

void AbstractShellView::OnMouseMove(MouseEvent *event) {
  if (nullptr == p_->shell_frame) {
    return;
  }

  AbstractView *view = nullptr;
  switch (p_->shell_frame->GetMouseLocation(event)) {
    case kResizeTop: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorTop));
      break;
    }
    case kResizeBottom: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorBottom));
      break;
    }
    case kResizeLeft: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorLeft));
      break;
    }
    case kResizeRight: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorRight));
      break;
    }
    case kResizeTopLeft: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorTopLeft));
      break;
    }
    case kResizeTopRight: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorTopRight));
      break;
    }
    case kResizeBottomLeft: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorBottomLeft));
      break;
    }
    case kResizeBottomRight: {
      ClearMouseTasks();
      event->SetCursor(Display::cursor(kCursorBottomRight));
      break;
    }
    case kLeftSide: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kLeft);
      break;
    }
    case kTopSide: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kTop);
      break;
    }
    case kRightSide: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kRight);
      break;
    }
    case kBottomSide: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      view = p_->shell_frame->GetViewAt(AbstractShellFrame::kBottom);
      break;
    }
    case kClientArea: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      view = p_->client_view;
      break;
    }
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }

  if (view) {
    Point cursor = event->GetWindowXY();
    MouseTaskIterator it(this);

    if (nullptr == it.next()) {
      DBG_ASSERT(it.mouse_task()->event_handler == this);
      DBG_ASSERT(nullptr == it.previous());
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

  // Now dispatch mouse move event:
//      task = static_cast<ViewTask *>(handler->p_->mouse_motion_task.next());
//      p_->mouse_event->response_ = InputEvent::kUnknown;
//      while (task) {
//        task->view->OnMouseMove(p_->mouse_event);
//        if (!p_->mouse_event->IsAccepted()) break;
//        task = static_cast<ViewTask *>(task->next());
//      }
}

void AbstractShellView::OnMouseButton(MouseEvent *event) {
  if ((event->GetButton() == MouseButton::kLeft) &&
      (event->GetState() == MouseButtonState::kPressed)) {

    if (p_->shell_frame) {
      int location = p_->shell_frame->GetMouseLocation(event);

      if (location == kTopSide && (nullptr == MouseTaskIterator(this).next())) {
        MoveWithMouse(event);
        event->Ignore();
        return;
      }

      if (location < kResizeMask) {
        ResizeWithMouse(event, (uint32_t) location);
        event->Ignore();
        return;
      }
    }
  }

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

void AbstractShellView::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void AbstractShellView::OnUpdate(AbstractView *view) {
  // override in sub class
}

Surface *AbstractShellView::GetSurface(const AbstractView * /* view */) const {
  return p_->shell_surface;
}

void AbstractShellView::OnDraw(const Context *context) {
  if (p_->shell_frame) p_->shell_frame->OnDraw(context);
}

void AbstractShellView::OnMaximized(bool maximized) {

}

void AbstractShellView::OnFullscreen(bool) {

}

void AbstractShellView::OnFocus(bool focus) {
  if (p_->shell_frame) {
    OnUpdate(nullptr);
  }
}

void AbstractShellView::OnViewAttached(AbstractView *view) {

}

void AbstractShellView::OnViewDetached(AbstractView *view) {

}

void AbstractShellView::MoveWithMouse(MouseEvent *event) const {
  ToplevelShellSurface::Get(p_->shell_surface)->Move(event->GetSeat(), event->GetSerial());
}

void AbstractShellView::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  ToplevelShellSurface::Get(p_->shell_surface)->Resize(event->GetSeat(), event->GetSerial(), edges);
}

AbstractShellFrame *AbstractShellView::GetShellFrame() const {
  return p_->shell_frame;
}

Surface *AbstractShellView::GetShellSurface() const {
  return p_->shell_surface;
}

void AbstractShellView::DrawShellFrame(AbstractShellFrame *window_frame, const Context *context) {
  window_frame->OnDraw(context);
}

void AbstractShellView::RecursiveUpdate(AbstractView *view) {
  view->RecursiveUpdate();
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

void AbstractShellView::OnXdgSurfaceConfigure(uint32_t serial) {
  ShellSurface *shell_surface = ShellSurface::Get(p_->shell_surface);
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
    DBG_ASSERT(min.width < max.width && min.height < max.height);

    width = clamp(width, min.width, max.width);
    height = clamp(height, min.height, max.height);
    if (width == p_->size.width && height == p_->size.height)
      do_resize = false;
  } else {
    // Initialize
    width = p_->size.width;
    height = p_->size.height;
  }

  if (do_resize) {
    ShellSurface::Get(p_->shell_surface)->ResizeWindow(width, height);  // Call xdg surface api
    OnResize(p_->size.width, p_->size.height, width, height);
    p_->size.width = width;
    p_->size.height = height;

    if (p_->shell_frame) {
      p_->shell_frame->OnResize(width, height);
    }
    if (p_->client_view) {
      SetContentViewGeometry();
    }

    // surface size is changed, reset the pointer position and enter/leave widgets
    ClearMouseTasks();
  }

  if (focus != IsFocused()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskFocused);
    OnFocus(focus);
  }

  if (maximized != IsMaximized()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskMaximized);
    OnMaximized(maximized);
  }

  if (fullscreen != IsFullscreen()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskFullscreen);
    OnFullscreen(fullscreen);
  }

  if (resizing != IsResizing()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskResizing);
  }
}

void AbstractShellView::OnXdgToplevelClose() {
  Close();
}

void AbstractShellView::OnAction(int action, SLOT slot) {
  ToplevelShellSurface *toplevel = ToplevelShellSurface::Get(p_->shell_surface);

  switch (action) {
    case kClose: {
      Close(slot);
      break;
    }
    case kMaximize: {
      if (IsMaximized()) {
        toplevel->UnsetMaximized();
      } else {
        toplevel->SetMaximized();
      }
      break;
    }
    case kMinimize: {
      Bit::Set<int>(p_->flags, Private::kFlagMaskMinimized);
      toplevel->SetMinimized();
      DBG_ASSERT(IsMinimized());
      break;
    }
    case kMenu: {
      fprintf(stderr, "menu\n");
      break;
    }
    default: break;
  }
}

void AbstractShellView::OnClientViewDestroyed(AbstractView *view, SLOT slot) {
  DBG_ASSERT(view == p_->client_view);
  p_->client_view = nullptr;
}

void AbstractShellView::SetContentViewGeometry() {
  Rect rect = GetClientGeometry(p_->size.width, p_->size.height);

  p_->client_view->MoveTo((int) rect.x(), (int) rect.y());
  p_->client_view->Resize((int) rect.width(), (int) rect.height());
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *parent, MouseEvent *event, MouseTaskIterator &tail) {
  AbstractView *sub = parent->DispatchMouseEnterEvent(event);
  while (sub) {
    DBG_ASSERT(sub != parent);
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

void AbstractShellView::ClearMouseTasks() {
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

void AbstractShellView::AttachViewsOnFrame() {
  AbstractView *view = nullptr;
  for (int i = 0; i <= AbstractShellFrame::Position::kBottom; i++) {
    view = p_->shell_frame->GetViewAt(static_cast<AbstractShellFrame::Position>(i));
    if (view) AttachView(view);
  }
}

void AbstractShellView::DetachViewsOnFrame() {
  AbstractView *view = nullptr;
  for (int i = 0; i <= AbstractShellFrame::Position::kBottom; i++) {
    view = p_->shell_frame->GetViewAt(static_cast<AbstractShellFrame::Position>(i));
    if (view) DetachView(view);
  }
}

}
