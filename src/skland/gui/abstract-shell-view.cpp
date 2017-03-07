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

#include <skland/gui/surface.hpp>
#include <skland/gui/shell-surface.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>

#include <skland/gui/abstract-shell-frame.hpp>

#include <skland/stock/theme.hpp>

#include "internal/abstract-view-private.hpp"
#include "internal/abstract-view-iterators.hpp"
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
  p_.reset(new Private);
  p_->size_.width = width;
  p_->size_.height = height;
  p_->parent_ = parent;

  if (title) p_->title_ = title;

  if (nullptr == p_->parent_) {
    p_->shell_surface_ = ToplevelShellSurface::Create(this, Theme::shadow_margin());
    ShellSurface::Get(p_->shell_surface_)->configure().Set(this, &AbstractShellView::OnXdgSurfaceConfigure);
    ToplevelShellSurface *toplevel = ToplevelShellSurface::Get(p_->shell_surface_);
    toplevel->configure().Set(this, &AbstractShellView::OnXdgToplevelConfigure);
    toplevel->close().Set(this, &AbstractShellView::OnXdgToplevelClose);
    toplevel->SetTitle(title);
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
  p_->shell_surface_->SetInputRegion(input_region);

  SetShellFrame(frame);
}

AbstractShellView::~AbstractShellView() {
  SetClientView(nullptr);

  if (p_->shell_frame_) {
    p_->shell_frame_->shell_view_ = nullptr;
    Theme::DestroyWindowFrame(p_->shell_frame_);
  }

  delete p_->shell_surface_;
}

void AbstractShellView::SetTitle(const char *title) {
  p_->title_ = title;
  if (nullptr == p_->parent_) {
    ToplevelShellSurface::Get(p_->shell_surface_)->SetTitle(title);
  }
}

void AbstractShellView::SetAppId(const char *app_id) {
  p_->app_id_ = app_id;
  if (nullptr == p_->parent_) {
    ToplevelShellSurface::Get(p_->shell_surface_)->SetAppId(app_id);
  }
}

void AbstractShellView::SetShellFrame(AbstractShellFrame *shell_frame) {
  if (shell_frame == p_->shell_frame_)
    return;

  if (p_->shell_frame_) {
    if (p_->shell_frame_->title_view_) DetachView(p_->shell_frame_->title_view_);
    p_->shell_frame_->shell_view_ = nullptr;
    delete p_->shell_frame_;
    p_->shell_frame_ = nullptr;
  }

  p_->shell_frame_ = shell_frame;
  // TODO: check if there's original window using the window frame

  if (p_->shell_frame_) {
    p_->shell_frame_->shell_view_ = this;
    if (p_->shell_frame_->title_view_) {
      AttachView(p_->shell_frame_->title_view_);
    }
    p_->shell_frame_->window_action().Connect(this, &AbstractShellView::OnWindowAction);

    p_->shell_frame_->OnSetup();
//    shell_frame_->OnResize(size_.width, size_.height);
  }

  OnUpdate(nullptr);
}

void AbstractShellView::SetClientView(AbstractView *view) {
  if (p_->client_view_ == view) return;

  if (p_->client_view_) {
    DetachView(p_->client_view_);
    p_->client_view_->destroyed().DisconnectAll(this, &AbstractShellView::OnContentViewDestroyed);
    delete p_->client_view_;
  }

  p_->client_view_ = view;

  if (p_->client_view_) {
    AttachView(p_->client_view_);
    p_->client_view_->destroyed().Connect(this, &AbstractShellView::OnContentViewDestroyed);
    SetContentViewGeometry();
  }
}

void AbstractShellView::Show() {
  if (!IsShown()) {
    p_->shell_surface_->Commit();
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
  return p_->title_;
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
  if (nullptr == p_->shell_frame_) {
    return kInterior;
  }

  return p_->shell_frame_->GetMouseLocation(event);
}

bool AbstractShellView::IsFullscreen() const {
  return 0 != (p_->flags_ & kFlagMaskFullscreen);
}

bool AbstractShellView::IsMaximized() const {
  return 0 != (p_->flags_ & kFlagMaskMaximized);
}

bool AbstractShellView::IsMinimized() const {
  return 0 != (p_->flags_ & kFlagMaskMinimized);
}

bool AbstractShellView::IsFocused() const {
  return 0 != (p_->flags_ & kFlagMaskFocused);
}

bool AbstractShellView::IsResizing() const {
  return 0 != (p_->flags_ & kFlagMaskResizing);
}

bool AbstractShellView::IsFrameless() const {
  return nullptr == p_->shell_frame_;
}

bool AbstractShellView::IsShown() const {
  return 0 != (p_->flags_ & kFlagMaskShown);
}

const Size &AbstractShellView::GetSize() const {
  return p_->size_;
}

AbstractView *AbstractShellView::GetClientView() const {
  return p_->client_view_;
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

Rect AbstractShellView::GetClientGeometry() const {
  if (IsFrameless()) {
    DBG_ASSERT(nullptr == p_->shell_frame_);
    return Rect::FromXYWH(0.f, 0.f, p_->size_.width, p_->size_.height);
  }

  return p_->shell_frame_->GetContentGeometry();
}

void AbstractShellView::OnMouseEnter(MouseEvent *event) {
  if (nullptr == p_->shell_frame_) {

    return;
  }

  switch (p_->shell_frame_->GetMouseLocation(event)) {
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
    case kTitleBar: {
      AbstractView *title_bar = p_->shell_frame_->title_view_;
      if (nullptr == title_bar) break;

      MouseTaskIterator it(this);
      while (it.next()) ++it; // move to tail
      DBG_ASSERT(nullptr == it.next());

      if (title_bar->Contain((int) event->window_x(), (int) event->window_y())) {
        title_bar->OnMouseEnter(event);
        if (event->IsAccepted()) {
          it.PushBack(title_bar);
          ++it;
          DispatchMouseEnterEvent(title_bar, event, it.mouse_task());
        }
      }

      break;
    }
    case kClientArea: {
      if (nullptr == p_->client_view_) break;

      MouseTaskIterator it(this);
      while (it.next()) ++it; // move to tail
      DBG_ASSERT(nullptr == it.next());

      if (p_->client_view_->Contain((int) event->window_x(), (int) event->window_y())) {
        p_->client_view_->OnMouseEnter(event);
        if (event->IsAccepted()) {
          it.PushBack(p_->client_view_);
          ++it;
          DispatchMouseEnterEvent(p_->client_view_, event, it.mouse_task());
        }
      }

      break;
    }
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }
}

void AbstractShellView::OnMouseLeave(MouseEvent *event) {
  MouseTaskIterator it(this);
  bool need_call = true;
  EventTask *task = nullptr;
  ++it;

  while (it) {
    task = it.mouse_task();
    ++it;

    task->Unlink();
    if (need_call) {
      static_cast<AbstractView *>(task->event_handler)->OnMouseLeave(event);
      if (event->IsRejected()) need_call = false;
    }
  }
}

void AbstractShellView::OnMouseMove(MouseEvent *event) {
  if (nullptr == p_->shell_frame_) {

    return;
  }

  switch (p_->shell_frame_->GetMouseLocation(event)) {
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
    case kTitleBar: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));

      AbstractView *title_view = p_->shell_frame_->title_view_;
      if (nullptr == title_view) break;

      MouseTaskIterator it(this);

      if (nullptr == it.next()) {
        DBG_ASSERT(it.mouse_task()->event_handler == this);
        DBG_ASSERT(nullptr == it.previous());
        if (title_view->Contain((int) event->window_x(), (int) event->window_y())) {
          title_view->OnMouseEnter(event);
          if (event->IsAccepted()) {
            it.PushBack(title_view);
            ++it;
            DispatchMouseEnterEvent(title_view, event, it.mouse_task());
          } else if (event->IsIgnored()) {
            DispatchMouseEnterEvent(title_view, event, it.mouse_task());
          }
        }
      } else {
        while (it.next()) ++it; // move to tail
        AbstractView *view = nullptr;
        EventTask *tail = nullptr;
        while (it.previous()) {
          tail = it.mouse_task();
          view = static_cast<AbstractView *>(tail->event_handler);
          if (view->Contain((int) event->window_x(), (int) event->window_y())) {
            break;
          }
          --it;
          tail->Unlink();
          static_cast<AbstractView *>(tail->event_handler)->OnMouseLeave(event);
          // TODO: if need to check the response

          if (nullptr == it.previous()) break;
        }
        DispatchMouseEnterEvent(view, event, tail);
      }
      break;
    }
    case kClientArea: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));

      if (nullptr == p_->client_view_) break;

      MouseTaskIterator it(this);

      if (nullptr == it.next()) {
        DBG_ASSERT(it.mouse_task()->event_handler == this);
        DBG_ASSERT(nullptr == it.previous());
        if (p_->client_view_->Contain((int) event->window_x(), (int) event->window_y())) {
          p_->client_view_->OnMouseEnter(event);
          if (event->IsAccepted()) {
            it.PushBack(p_->client_view_);
            ++it;
            DispatchMouseEnterEvent(p_->client_view_, event, it.mouse_task());
          } else if (event->IsIgnored()) {
            DispatchMouseEnterEvent(p_->client_view_, event, it.mouse_task());
          }
        }
      } else {
        while (it.next()) ++it; // move to tail
        AbstractView *view = nullptr;
        EventTask *tail = nullptr;
        while (it.previous()) {
          tail = it.mouse_task();
          view = static_cast<AbstractView *>(tail->event_handler);
          if (view->Contain((int) event->window_x(), (int) event->window_y())) {
            break;
          }
          --it;
          tail->Unlink();
          static_cast<AbstractView *>(tail->event_handler)->OnMouseLeave(event);
          // TODO: if need to check the response

          if (nullptr == it.previous()) break;
        }
        DispatchMouseEnterEvent(view, event, tail);
      }

      break;
    }
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
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
  if ((event->button() == kMouseButtonLeft) &&
      (event->state() == kMouseButtonPressed)) {

    if (p_->shell_frame_) {
      int location = p_->shell_frame_->GetMouseLocation(event);

      if (location == kTitleBar && (nullptr == MouseTaskIterator(this).next())) {
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
  return p_->shell_surface_;
}

void AbstractShellView::OnDraw(const Context *context) {
  if (p_->shell_frame_) p_->shell_frame_->OnDraw(context);
}

void AbstractShellView::OnMaximized(bool maximized) {

}

void AbstractShellView::OnFullscreen(bool) {

}

void AbstractShellView::OnFocus(bool focus) {
  if (p_->shell_frame_) {
    OnUpdate(nullptr);
  }
}

void AbstractShellView::OnViewAttached(AbstractView *view) {

}

void AbstractShellView::OnViewDetached(AbstractView *view) {

}

void AbstractShellView::MoveWithMouse(MouseEvent *event) const {
  ToplevelShellSurface::Get(p_->shell_surface_)->Move(event->GetSeat(), event->serial());
}

void AbstractShellView::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  ToplevelShellSurface::Get(p_->shell_surface_)->Resize(event->GetSeat(), event->serial(), edges);
}

AbstractShellFrame *AbstractShellView::shell_frame() const {
  return p_->shell_frame_;
}

Surface *AbstractShellView::shell_surface() const {
  return p_->shell_surface_;
}

void AbstractShellView::ResizeShellFrame(AbstractShellFrame *window_frame, int width, int height) {
  window_frame->OnResize(width, height);
}

void AbstractShellView::DrawShellFrame(AbstractShellFrame *window_frame, const Context *context) {
  window_frame->OnDraw(context);
}

void AbstractShellView::UpdateAll(AbstractView *view) {
  view->UpdateAll();
}

void AbstractShellView::OnXdgSurfaceConfigure(uint32_t serial) {
  ShellSurface *shell_surface = ShellSurface::Get(p_->shell_surface_);
  shell_surface->AckConfigure(serial);

  if (!IsShown()) {
    Bit::Set<int>(p_->flags_, kFlagMaskShown);
    shell_surface->ResizeWindow(p_->size_.width, p_->size_.height);
    OnShown();
  }
}

void AbstractShellView::OnXdgToplevelConfigure(int width, int height, int states) {
  using wayland::XdgToplevel;

  bool maximized = ((states & XdgToplevel::kStateMaskMaximized) != 0);
  bool fullscreen = ((states & XdgToplevel::kStateMaskFullscreen) != 0);
  bool resizing = ((states & XdgToplevel::kStateMaskResizing) != 0);
  bool focus = ((states & XdgToplevel::kStateMaskActivated) != 0);

  bool do_resize = true;

  if (width > 0 && height > 0) {
    Size min = this->GetMinimalSize();
    Size max = this->GetMaximalSize();
    DBG_ASSERT(min.width < max.width && min.height < max.height);

    width = clamp(width, min.width, max.width);
    height = clamp(height, min.height, max.height);
    if (width == p_->size_.width && height == p_->size_.height) do_resize = false;
  } else {
    // Initialize
    width = p_->size_.width;
    height = p_->size_.height;
  }

  if (do_resize) {
    ShellSurface::Get(p_->shell_surface_)->ResizeWindow(width, height);
    p_->size_.width = width;
    p_->size_.height = height;
    OnSizeChanged(width, height);

    // Resize frame first, then the content view
    if (p_->shell_frame_) {
      p_->shell_frame_->OnResize(width, height);
    }
    if (p_->client_view_) {
      SetContentViewGeometry();
    }
  }

  if (focus != IsFocused()) {
    Bit::Inverse<int>(p_->flags_, kFlagMaskFocused);
    OnFocus(focus);
  }

  if (maximized != IsMaximized()) {
    Bit::Inverse<int>(p_->flags_, kFlagMaskMaximized);
    OnMaximized(maximized);
  }

  if (fullscreen != IsFullscreen()) {
    Bit::Inverse<int>(p_->flags_, kFlagMaskFullscreen);
    OnFullscreen(fullscreen);
  }

  if (resizing != IsResizing()) {
    Bit::Inverse<int>(p_->flags_, kFlagMaskResizing);
  }
}

void AbstractShellView::OnXdgToplevelClose() {
  Close();
}

void AbstractShellView::OnWindowAction(int action, SLOT slot) {
  ToplevelShellSurface *toplevel = ToplevelShellSurface::Get(p_->shell_surface_);

  switch (action) {
    case kActionClose: {
      Close(slot);
      break;
    }
    case kActionMaximize: {
      if (IsMaximized()) {
        toplevel->UnsetMaximized();
      } else {
        toplevel->SetMaximized();
      }
      break;
    }
    case kActionMinimize: {
      Bit::Set<int>(p_->flags_, kFlagMaskMinimized);
      toplevel->SetMinimized();
      DBG_ASSERT(IsMinimized());
      break;
    }
    case kActionMenu: {
      fprintf(stderr, "menu\n");
      break;
    }
    default: break;
  }
}

void AbstractShellView::OnContentViewDestroyed(AbstractView *view, SLOT slot) {
  DBG_ASSERT(view == p_->client_view_);
  p_->client_view_ = nullptr;
}

void AbstractShellView::SetContentViewGeometry() {
  Rect rect = GetClientGeometry();

  p_->client_view_->MoveTo((int) rect.x(), (int) rect.y());
  p_->client_view_->Resize((int) rect.width(), (int) rect.height());
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *parent, MouseEvent *event, EventTask *tail) {
  DBG_ASSERT(nullptr == tail->next());
  AbstractView::Iterator it(parent);
  for (it = it.first_child(); it; ++it) {
    if (it.view()->Contain((int) event->window_x(), (int) event->window_y())) {
      it.view()->OnMouseEnter(event);
      if (event->IsAccepted()) {
        tail->PushBack(MouseTaskIterator(it.view()).mouse_task());
        tail = static_cast<EventTask *>(tail->next());
        DispatchMouseEnterEvent(it.view(), event, tail);
      } else if (event->IsIgnored()) {
        DispatchMouseEnterEvent(it.view(), event, tail);
      }
      break;
    }
  }
}

}
