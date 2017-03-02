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

#include <skland/gui/abstract-window.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/gui/surface.hpp>
#include <skland/gui/shell-surface.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>

#include <skland/gui/abstract-window-frame.hpp>

#include "internal/abstract-view-private.hpp"
#include "internal/display-proxy.hpp"
#include "internal/mouse-task-proxy.hpp"

namespace skland {

AbstractWindow::AbstractWindow(const char *title, AbstractWindow *parent,
                               AbstractWindowFrame *frame)
    : AbstractWindow(400, 300, title, parent, frame) {
}

AbstractWindow::AbstractWindow(int width,
                               int height,
                               const char *title,
                               AbstractWindow *parent,
                               AbstractWindowFrame *frame)
    : AbstractEventHandler(),
      shown_(false),
      flags_(0),
      shell_surface_(nullptr),
      window_frame_(nullptr),
      size_(width, height),
      title_bar_(nullptr),
      content_view_(nullptr),
      parent_(parent) {
  if (title) title_ = title;

  if (nullptr == parent_) {
    shell_surface_ = ToplevelShellSurface::Create(this, Theme::shadow_margin());
    ShellSurface::Get(shell_surface_)->configure().Set(this, &AbstractWindow::OnXdgSurfaceConfigure);
    ToplevelShellSurface *toplevel = ToplevelShellSurface::Get(shell_surface_);
    toplevel->configure().Set(this, &AbstractWindow::OnXdgToplevelConfigure);
    toplevel->close().Set(this, &AbstractWindow::OnXdgToplevelClose);
    toplevel->SetTitle(title);
  } else {
    // TODO: create popup shell surface
  }

  int x = 0, y = 0;  // The input region
  x += Theme::shadow_margin().left - AbstractWindowFrame::kResizingMargin.left;
  y += Theme::shadow_margin().top - AbstractWindowFrame::kResizingMargin.top;
  width += AbstractWindowFrame::kResizingMargin.lr();
  height += AbstractWindowFrame::kResizingMargin.tb();

  wayland::Region input_region;
  input_region.Setup(DisplayProxy().wl_compositor());
  input_region.Add(x, y, width, height);
  shell_surface_->SetInputRegion(input_region);

  SetWindowFrame(frame);
}

AbstractWindow::~AbstractWindow() {
  SetContentView(nullptr);

  Theme::DestroyWindowFrame(window_frame_);
  delete shell_surface_;
}

void AbstractWindow::SetTitle(const char *title) {
  title_ = title;
  if (nullptr == parent_) {
    ToplevelShellSurface::Get(shell_surface_)->SetTitle(title);
  }
}

void AbstractWindow::SetAppId(const char *app_id) {
  app_id_ = app_id;
  if (nullptr == parent_) {
    ToplevelShellSurface::Get(shell_surface_)->SetAppId(app_id);
  }
}

void AbstractWindow::SetWindowFrame(AbstractWindowFrame *window_frame) {
  if (window_frame == window_frame_)
    return;

  if (window_frame_) {
    if (title_bar_) DetachView(title_bar_);
    delete window_frame_;
    title_bar_ = nullptr;
    window_frame_ = nullptr;
  }

  window_frame_ = window_frame;
  // TODO: check if there's original window using the window frame

  if (window_frame_) {
    window_frame_->window_ = this;
    title_bar_ = window_frame_->GetContainer();
    if (title_bar_) AttachView(title_bar_);
    window_frame_->window_action().Connect(this, &AbstractWindow::OnWindowAction);
    window_frame_->OnResize(size_.width, size_.height);
  }
}

void AbstractWindow::SetContentView(AbstractView *view) {
  if (content_view_ == view) return;

  if (content_view_) {
    DetachView(content_view_);
    delete content_view_;
  }

  content_view_ = view;

  if (content_view_) {
    AttachView(content_view_);
    SetContentViewGeometry();
  }
}

void AbstractWindow::Show() {
  if (!shown_) {
    shell_surface_->Commit();
  }
}

void AbstractWindow::Close(SLOT) {
  if (Surface::GetShellSurfaceCount() == 1) {
    Application::Exit();
  }

  // TODO: use a close task if there's more than 1 windows in an application

  // windows will be deleted when application exits, uncomment this line
  // sometimes cause segfault when close button is clicked:
  //  delete this;
}

void AbstractWindow::Maximize(SLOT) {

}

void AbstractWindow::Minimize(SLOT) {

}

Size AbstractWindow::GetMinimalSize() const {
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

Size AbstractWindow::GetPreferredSize() const {
  return Size(640, 480);
}

Size AbstractWindow::GetMaximalSize() const {
  return Size(65536, 65536);
}

int AbstractWindow::GetMouseLocation(const MouseEvent *event) const {
  if (IsFrameless()) return kInterior;

  return window_frame_->GetMouseLocation(event);
}

Rect AbstractWindow::GetClientGeometry() const {
  if (IsFrameless()) {
    DBG_ASSERT(nullptr == window_frame_);
    return Rect::FromXYWH(0.f, 0.f, size_.width, size_.height);
  }

  return window_frame_->GetClientGeometry();
}

void AbstractWindow::OnMouseEnter(MouseEvent *event) {
  if (!IsFrameless()) {
    switch (window_frame_->GetMouseLocation(event)) {
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
  event->Accept();
}

void AbstractWindow::OnMouseLeave(MouseEvent *event) {
  event->Accept();
}

void AbstractWindow::OnMouseMove(MouseEvent *event) {
  if (!IsFrameless()) {
    switch (window_frame_->GetMouseLocation(event)) {
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

void AbstractWindow::OnMouseButton(MouseEvent *event) {
  if ((event->button() == kMouseButtonLeft) &&
      (event->state() == kMouseButtonPressed)) {

    if (!IsFrameless()) {
      int location = window_frame_->GetMouseLocation(event);

      if (location == kTitleBar) {
        MouseTaskProxy proxy(this);
        if (proxy.GetNextTask()) {
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

void AbstractWindow::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void AbstractWindow::OnUpdate(AbstractView *view) {
  // override in sub class
}

Surface *AbstractWindow::GetSurface(const AbstractView * /* view */) const {
  return shell_surface_;
}

void AbstractWindow::OnDraw(const Context *context) {
  if (window_frame_) {
    window_frame_->OnDraw(context);
  }
}

void AbstractWindow::OnViewDestroyed(AbstractView *view) {
  if (view == title_bar_) {
    title_bar_ = nullptr;
  } else if (view == content_view_) {
    content_view_ = nullptr;
  }
}

void AbstractWindow::OnMaximized(bool maximized) {

}

void AbstractWindow::OnFullscreen(bool) {

}

void AbstractWindow::OnFocus(bool focus) {

}

void AbstractWindow::OnViewAttached(AbstractView *view) {
  // override in subclass
}

void AbstractWindow::OnViewDetached(AbstractView *view) {
  // override in subclass
}

void AbstractWindow::MoveWithMouse(MouseEvent *event) const {
  ToplevelShellSurface::Get(shell_surface_)->Move(event->GetSeat(), event->serial());
}

void AbstractWindow::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  ToplevelShellSurface::Get(shell_surface_)->Resize(event->GetSeat(), event->serial(), edges);
}

void AbstractWindow::ResizeWindowFrame(AbstractWindowFrame *window_frame, int width, int height) {
  window_frame->OnResize(width, height);
}

void AbstractWindow::DrawWindowFrame(AbstractWindowFrame *window_frame, const Context *context) {
  window_frame->OnDraw(context);
}

void AbstractWindow::OnXdgSurfaceConfigure(uint32_t serial) {
  ShellSurface *shell_surface = ShellSurface::Get(shell_surface_);
  shell_surface->AckConfigure(serial);

  if (!shown_) {
    shown_ = true;
    shell_surface->ResizeWindow(size_.width, size_.height);
    OnShown();
  }
}

void AbstractWindow::OnXdgToplevelConfigure(int width, int height, int states) {
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
    if (window_frame_) window_frame_->OnResize(width, height);
    if (content_view_) SetContentViewGeometry();
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

void AbstractWindow::OnXdgToplevelClose() {
  Close();
}

void AbstractWindow::OnWindowAction(int action, SLOT slot) {
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

void AbstractWindow::SetContentViewGeometry() {
  Rect rect = GetClientGeometry();

  content_view_->MoveTo((int) rect.x(), (int) rect.y());
  content_view_->Resize((int) rect.width(), (int) rect.height());
}

}
