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

#include <skland/gui/abstract-shell-view.hpp>

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
    : AbstractEventHandler(),
      flags_(0),
      shell_surface_(nullptr),
      shell_frame_(nullptr),
      size_(width, height),
      content_view_(nullptr),
      parent_(parent) {
  if (title) title_ = title;

  if (nullptr == parent_) {
    shell_surface_ = ToplevelShellSurface::Create(this, Theme::shadow_margin());
    ShellSurface::Get(shell_surface_)->configure().Set(this, &AbstractShellView::OnXdgSurfaceConfigure);
    ToplevelShellSurface *toplevel = ToplevelShellSurface::Get(shell_surface_);
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
  shell_surface_->SetInputRegion(input_region);

  SetShellFrame(frame);
}

AbstractShellView::~AbstractShellView() {
  SetContentView(nullptr);

  if (shell_frame_) {
    shell_frame_->shell_view_ = nullptr;
    Theme::DestroyWindowFrame(shell_frame_);
  }

  delete shell_surface_;
}

void AbstractShellView::SetTitle(const char *title) {
  title_ = title;
  if (nullptr == parent_) {
    ToplevelShellSurface::Get(shell_surface_)->SetTitle(title);
  }
}

void AbstractShellView::SetAppId(const char *app_id) {
  app_id_ = app_id;
  if (nullptr == parent_) {
    ToplevelShellSurface::Get(shell_surface_)->SetAppId(app_id);
  }
}

void AbstractShellView::SetShellFrame(AbstractShellFrame *shell_frame) {
  if (shell_frame == shell_frame_)
    return;

  if (shell_frame_) {
    if (shell_frame_->title_bar_) DetachView(shell_frame_->title_bar_);
    shell_frame_->shell_view_ = nullptr;
    delete shell_frame_;
    shell_frame_ = nullptr;
  }

  shell_frame_ = shell_frame;
  // TODO: check if there's original window using the window frame

  if (shell_frame_) {
    shell_frame_->shell_view_ = this;
    if (shell_frame_->title_bar_) {
      AttachView(shell_frame_->title_bar_);
    }
    shell_frame_->window_action().Connect(this, &AbstractShellView::OnWindowAction);
    shell_frame_->OnResize(size_.width, size_.height);
  }

  OnUpdate(nullptr);
}

void AbstractShellView::SetContentView(AbstractView *view) {
  if (content_view_ == view) return;

  if (content_view_) {
    DetachView(content_view_);
    content_view_->destroyed().DisconnectAll(this, &AbstractShellView::OnContentViewDestroyed);
    delete content_view_;
  }

  content_view_ = view;

  if (content_view_) {
    AttachView(content_view_);
    content_view_->destroyed().Connect(this, &AbstractShellView::OnContentViewDestroyed);
    SetContentViewGeometry();
  }
}

void AbstractShellView::Show() {
  if (!IsShown()) {
    shell_surface_->Commit();
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
  if (IsFrameless()) return kInterior;

  return shell_frame_->GetMouseLocation(event);
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
    DBG_ASSERT(nullptr == shell_frame_);
    return Rect::FromXYWH(0.f, 0.f, size_.width, size_.height);
  }

  return shell_frame_->GetContentGeometry();
}

void AbstractShellView::OnMouseEnter(MouseEvent *event) {
  if (!IsFrameless()) {
    switch (shell_frame_->GetMouseLocation(event)) {
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
      default: {
        event->SetCursor(Display::cursor(kCursorLeftPtr));
        break;
      }
    }
  }

  MouseTaskIterator proxy(this);
  DBG_ASSERT(nullptr == proxy.next());

  if (shell_frame_ &&
      shell_frame_->title_bar() &&
      shell_frame_->title_bar()->Contain((int) event->window_x(), (int) event->window_y())) {
    shell_frame_->title_bar()->OnMouseEnter(event);
    if (event->IsAccepted()) {
      proxy.PushBack(shell_frame_->title_bar());
      ++proxy;
      DispatchMouseEnterEvent(shell_frame_->title_bar(), event, proxy.mouse_task());
    }
  } else if (content_view_ && content_view_->Contain((int) event->window_x(), (int) event->window_y())) {
    content_view_->OnMouseEnter(event);
    if (event->IsAccepted()) {
      proxy.PushBack(content_view_);
      ++proxy;
      DispatchMouseEnterEvent(content_view_, event, proxy.mouse_task());
    }
  }
}

void AbstractShellView::OnMouseLeave(MouseEvent *event) {
  MouseTaskIterator proxy(this);
  bool need_call = true;
  EventTask *tmp = proxy.mouse_task();

  while (++proxy) {
    tmp->Unlink();
    tmp = proxy.mouse_task();
    if (need_call) {
      static_cast<AbstractView *>(proxy.mouse_task()->event_handler)->OnMouseLeave(event);
      if (!event->IsAccepted()) need_call = false;
    }
  }
}

void AbstractShellView::OnMouseMove(MouseEvent *event) {
  if (!IsFrameless()) {
    switch (shell_frame_->GetMouseLocation(event)) {
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
      default: {
        event->SetCursor(Display::cursor(kCursorLeftPtr));
        break;
      }
    }
  }

  event->Accept();
}

void AbstractShellView::OnMouseButton(MouseEvent *event) {
  if ((event->button() == kMouseButtonLeft) &&
      (event->state() == kMouseButtonPressed)) {

    if (!IsFrameless()) {
      int location = shell_frame_->GetMouseLocation(event);

      if (location == kTitleBar) {
        MouseTaskIterator proxy(this);
        if (proxy.next()) {
          // If the mouse is hover on a sub widget (mostly close/min/max button on title bar).
          event->Accept();
          return;
        }

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

  event->Accept();
}

void AbstractShellView::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void AbstractShellView::OnUpdate(AbstractView *view) {
  // override in sub class
}

Surface *AbstractShellView::GetSurface(const AbstractView * /* view */) const {
  return shell_surface_;
}

void AbstractShellView::OnDraw(const Context *context) {
  if (shell_frame_) shell_frame_->OnDraw(context);
}

void AbstractShellView::OnMaximized(bool maximized) {

}

void AbstractShellView::OnFullscreen(bool) {

}

void AbstractShellView::OnFocus(bool focus) {
  if (shell_frame_) {
    OnUpdate(nullptr);
  }
}

void AbstractShellView::OnViewAttached(AbstractView *view) {

}

void AbstractShellView::OnViewDetached(AbstractView *view) {

}

void AbstractShellView::MoveWithMouse(MouseEvent *event) const {
  ToplevelShellSurface::Get(shell_surface_)->Move(event->GetSeat(), event->serial());
}

void AbstractShellView::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  ToplevelShellSurface::Get(shell_surface_)->Resize(event->GetSeat(), event->serial(), edges);
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
  ShellSurface *shell_surface = ShellSurface::Get(shell_surface_);
  shell_surface->AckConfigure(serial);

  if (!IsShown()) {
    Bit::Set<int>(flags_, kFlagMaskShown);
    shell_surface->ResizeWindow(size_.width, size_.height);
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
    if (width == size_.width && height == size_.height) do_resize = false;
  } else {
    // Initialize
    width = size_.width;
    height = size_.height;
  }

  if (do_resize) {
    ShellSurface::Get(shell_surface_)->ResizeWindow(width, height);
    size_.width = width;
    size_.height = height;
    OnSizeChanged(width, height);

    // Resize frame first, then the content view
    if (shell_frame_) {
      shell_frame_->OnResize(width, height);
    }
    if (content_view_) {
      SetContentViewGeometry();
    }
  }

  if (focus != IsFocused()) {
    Bit::Inverse<int>(flags_, kFlagMaskFocused);
    OnFocus(focus);
  }

  if (maximized != IsMaximized()) {
    Bit::Inverse<int>(flags_, kFlagMaskMaximized);
    OnMaximized(maximized);
  }

  if (fullscreen != IsFullscreen()) {
    Bit::Inverse<int>(flags_, kFlagMaskFullscreen);
    OnFullscreen(fullscreen);
  }

  if (resizing != IsResizing()) {
    Bit::Inverse<int>(flags_, kFlagMaskResizing);
  }
}

void AbstractShellView::OnXdgToplevelClose() {
  Close();
}

void AbstractShellView::OnWindowAction(int action, SLOT slot) {
  ToplevelShellSurface *toplevel = ToplevelShellSurface::Get(shell_surface_);

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
      Bit::Set<int>(flags_, kFlagMaskMinimized);
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
  DBG_ASSERT(view == content_view_);
  content_view_ = nullptr;
}

void AbstractShellView::SetContentViewGeometry() {
  Rect rect = GetClientGeometry();

  content_view_->MoveTo((int) rect.x(), (int) rect.y());
  content_view_->Resize((int) rect.width(), (int) rect.height());
}

void AbstractShellView::DispatchMouseEnterEvent(AbstractView *view, MouseEvent *event, EventTask *task) {
  AbstractView::Iterator it(view);
  for (it = it.first_child(); it; ++it) {
    if (it.view()->Contain((int) event->window_x(), (int) event->window_y())) {
      it.view()->OnMouseEnter(event);
      if (event->IsAccepted()) {
        task->PushBack(MouseTaskIterator(it.view()).mouse_task());
        task = static_cast<EventTask *>(task->next());
        DispatchMouseEnterEvent(it.view(), event, task);
      }
      break;
    }
  }
}

}
