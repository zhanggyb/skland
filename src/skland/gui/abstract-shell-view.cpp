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

#include "internal/abstract-shell-view_private.hpp"

#include <skland/core/assert.hpp>
#include "skland/numerical/bit.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/region.hpp>

#include <skland/graphic/canvas.hpp>

#include <skland/gui/theme.hpp>

#include "internal/abstract-view_private.hpp"

#include "SkCanvas.h"
#include "SkImage.h"

namespace skland {
namespace gui {

using core::SLOT;
using numerical::Bit;
using core::Margin;

const Margin AbstractShellView::kResizingMargin(5, 5, 5, 5);

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
    p_->shell_surface = Surface::Shell::Toplevel::Create(this, Theme::GetShadowMargin());
    Surface::Shell::Toplevel *top_level_role = Surface::Shell::Toplevel::Get(p_->shell_surface);
    top_level_role->SetTitle(title);
  } else {
    // TODO: create popup shell surface
  }

  int x = 0, y = 0;  // The input region
  x += Theme::GetShadowMargin().left - kResizingMargin.left;
  y += Theme::GetShadowMargin().top - kResizingMargin.top;
  width += kResizingMargin.lr();
  height += kResizingMargin.tb();

  Region input_region;
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

void AbstractShellView::ToggleFullscreen(const Output *output, SLOT) {
  Surface::Shell::Toplevel *toplevel = Surface::Shell::Toplevel::Get(p_->shell_surface);
  if (nullptr == toplevel) return;

  if (output) {
    toplevel->SetFullscreen(output);
  } else {
    toplevel->UnsetFullscreen();
  }
}

const std::string &AbstractShellView::GetTitle() const {
  return p_->title;
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

AbstractShellView *AbstractShellView::GetParent() const {
  return p_->parent;
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

void AbstractShellView::OnMouseDown(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnMouseUp(MouseEvent *event) {
  // override in sub class
}

void AbstractShellView::OnKeyDown(KeyEvent *event) {
  // override in sub class
}

void AbstractShellView::OnKeyUp(KeyEvent *event) {
  // override in sub class
}

void AbstractShellView::OnUpdate(AbstractView *view) {
  // override in sub class
}

Surface *AbstractShellView::GetSurface(const AbstractView * /* view */) const {
  return p_->shell_surface;
}

void AbstractShellView::OnEnterOutput(const Surface *surface, const Output *output) {
}

void AbstractShellView::OnLeaveOutput(const Surface *surface, const Output *output) {
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
  Surface::Shell::Toplevel::Get(p_->shell_surface)->Move(*event, event->GetSerial());
}

void AbstractShellView::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  Surface::Shell::Toplevel::Get(p_->shell_surface)->Resize(*event, event->GetSerial(), edges);
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

void AbstractShellView::DispatchUpdate(AbstractView *view) {
  view->Update();
  view->DispatchUpdate();
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
  bool maximized = (0 != (states & Surface::Shell::Toplevel::kStateMaskMaximized));
  bool fullscreen = (0 != (states & Surface::Shell::Toplevel::kStateMaskFullscreen));
  bool resizing = (0 != (states & Surface::Shell::Toplevel::kStateMaskResizing));
  bool focus = (0 != (states & Surface::Shell::Toplevel::kStateMaskActivated));

  if (maximized != IsMaximized()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskMaximized);
    OnMaximized(maximized);
  }

  if (fullscreen != IsFullscreen()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskFullscreen);
    OnFullscreen(fullscreen);
  }

  if (resizing != IsResizing()) {
    // TODO: no need to use this flag
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskResizing);
  }

  if (width > 0 && height > 0) {
    p_->dirty_flag = 1;
    Size saved_size = p_->size;
    p_->size.width = width;
    p_->size.height = height;
    if (!OnConfigureSize(p_->last_size, p_->size)) {
      p_->size = saved_size;
      p_->dirty_flag = 0;
    }
  }

  if (focus != IsFocused()) {
    Bit::Inverse<int>(p_->flags, Private::kFlagMaskFocused);
    OnFocus(focus);
  }
}

void AbstractShellView::OnXdgToplevelClose() {
  Close();
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *view, MouseEvent *event) {
  Point cursor = event->GetWindowXY();
  EventTask *mouse_task = EventTask::GetMouseTask(this);

  if (nullptr == mouse_task->next()) {
    _ASSERT(mouse_task->event_handler() == this);
    _ASSERT(nullptr == mouse_task->previous());
    if (view->Contain(cursor.x, cursor.y)) {
      view->OnMouseEnter(event);
      if (event->IsAccepted()) {
        mouse_task->PushBack(EventTask::GetMouseTask(view));
        mouse_task = static_cast<EventTask *>(mouse_task->next());
        DispatchMouseEnterEvent(view, event, mouse_task);
      } else if (event->IsIgnored()) {
        DispatchMouseEnterEvent(view, event, mouse_task);
      }
    }
  } else {
    while (mouse_task->next()) mouse_task = static_cast<EventTask *>(mouse_task->next()); // move to tail
    AbstractView *last = nullptr;
    EventTask *tail = nullptr;
    while (mouse_task->previous()) {
      tail = mouse_task;
      last = static_cast<AbstractView *>(tail->event_handler());
      if (last->Contain(cursor.x, cursor.y)) {
        break;
      }
      mouse_task = static_cast<EventTask *>(mouse_task->previous());
      tail->Unlink();
      last->OnMouseLeave();
      if (nullptr == mouse_task->previous()) break;
    }

    DispatchMouseEnterEvent(last, event, mouse_task);
  }
}

void AbstractShellView::DispatchMouseLeaveEvent() {
  Task *it = EventTask::GetMouseTask(this)->next();

  Task *tmp = nullptr;
  while (it) {
    tmp = it;
    it = it->next();
    tmp->Unlink();
    static_cast<EventTask *>(tmp)->event_handler()->OnMouseLeave();
  }
}

void AbstractShellView::DispatchMouseDownEvent(MouseEvent *event) {
  _ASSERT(event->GetState() == kMouseButtonPressed);

  Task *it = EventTask::GetMouseTask(this)->next();
  while (it) {
    static_cast<EventTask *>(it)->event_handler()->OnMouseDown(event);
    if (event->IsRejected()) break;
    it = it->next();
  }
}

void AbstractShellView::DispatchMouseUpEvent(MouseEvent *event) {
  _ASSERT(event->GetState() == kMouseButtonReleased);

  Task *it = EventTask::GetMouseTask(this)->next();
  while (it) {
    static_cast<EventTask *>(it)->event_handler()->OnMouseUp(event);
    if (event->IsRejected()) break;
    it = it->next();
  }
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *parent, MouseEvent *event, EventTask *tail) {
  AbstractView *sub = parent->DispatchMouseEnterEvent(event);
  while (sub) {
    _ASSERT(sub != parent);
    sub->OnMouseEnter(event);
    if (event->IsAccepted()) {
      tail->PushBack(EventTask::GetMouseTask(sub));
      tail = static_cast<EventTask *>(tail->next());
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

void AbstractShellView::DropShadow(const Context *context) {
  int scale = context->surface()->GetScale();
  float rad = (Theme::GetShadowRadius() - 1.f); // The spread radius
  float offset_x = Theme::GetShadowOffsetX();
  float offset_y = Theme::GetShadowOffsetY();

  int width = GetWidth();
  int height = GetHeight();

  if (!IsFocused()) {
    rad = (int) rad / 3;
    offset_x = (int) offset_x / 3;
    offset_y = (int) offset_y / 3;
  }

  // shadow map
  SkCanvas *c = context->canvas()->GetSkCanvas();
  c->save();
  c->scale(scale, scale);
  sk_sp<SkImage> image = SkImage::MakeFromRaster(*Theme::GetShadowPixmap(), nullptr, nullptr);

  // top-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 0,
                                    2 * Theme::GetShadowRadius(), 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(-rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // top
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::GetShadowRadius(), 0,
                                    250 - 2 * Theme::GetShadowRadius(), 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(rad + offset_x, -rad + offset_y,
                                    width - 2 * rad, 2 * rad),
                   nullptr);

  // top-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::GetShadowRadius(), 0,
                                    250, 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(width - rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 2 * Theme::GetShadowRadius(),
                                    2 * Theme::GetShadowRadius(), 250 - 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(-rad + offset_x, rad + offset_y,
                                    2 * rad, height - 2 * rad),
                   nullptr);

  // bottom-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 250 - 2 * Theme::GetShadowRadius(),
                                    2 * Theme::GetShadowRadius(), 250),
                   SkRect::MakeXYWH(-rad + offset_x, height - rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // bottom
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::GetShadowRadius(), 250 - 2 * Theme::GetShadowRadius(),
                                    250 - 2 * Theme::GetShadowRadius(), 250),
                   SkRect::MakeXYWH(rad + offset_x, height - rad + offset_y,
                                    width - 2 * rad, 2 * rad),
                   nullptr);

  // bottom-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::GetShadowRadius(), 250 - 2 * Theme::GetShadowRadius(),
                                    250, 250),
                   SkRect::MakeXYWH(width - rad + offset_x,
                                    height - rad + offset_y,
                                    2 * rad,
                                    2 * rad),
                   nullptr);

  // right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::GetShadowRadius(), 2 * Theme::GetShadowRadius(),
                                    250, 250 - 2 * Theme::GetShadowRadius()),
                   SkRect::MakeXYWH(width - rad + offset_x, rad + offset_y,
                                    2 * rad, height - 2 * rad),
                   nullptr);
  c->restore();
}

// ---------

void AbstractShellView::RedrawTask::Run() const {
  if (shell_view_->p_->dirty_flag) {
    shell_view_->OnSizeChange(shell_view_->p_->last_size, shell_view_->p_->size);
    shell_view_->p_->last_size = shell_view_->p_->size;
    shell_view_->p_->dirty_flag = 0;
  }

  shell_view_->OnDraw(&context);

  if (shell_view_->p_->is_damaged) {
    context.surface()->Damage(shell_view_->p_->damaged_region.x(),
                              shell_view_->p_->damaged_region.y(),
                              shell_view_->p_->damaged_region.width(),
                              shell_view_->p_->damaged_region.height());
    shell_view_->p_->is_damaged = false;
  }
}

AbstractShellView::RedrawTask *AbstractShellView::RedrawTask::Get(const AbstractShellView *shell_view) {
  return &shell_view->p_->redraw_task;
}

} // namespace gui
} // namespace skland
