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
#include <skland/gui/abstract-surface.hpp>
#include <skland/gui/abstract-window-frame.hpp>

#include "internal/view-task.hpp"

namespace skland {

AbstractWindow::AbstractWindow(const char *title,
                               AbstractWindowFrame *frame)
    : AbstractWindow(400, 300, title, frame) {
}

AbstractWindow::AbstractWindow(int width,
                               int height,
                               const char *title,
                               AbstractWindowFrame *frame)
    : AbstractView(width, height),
      flags_(0),
      window_frame_(nullptr),
      shell_surface_(nullptr) {
  if (title) title_ = title;
}

AbstractWindow::~AbstractWindow() {
  Theme::DestroyWindowFrame(window_frame_);
  delete shell_surface_;
}

void AbstractWindow::SetTitle(const char *title) {
  title_ = title;
  xdg_toplevel_.SetTitle(title);
}

void AbstractWindow::SetAppId(const char *app_id) {
  app_id_ = app_id;
  xdg_toplevel_.SetAppId(app_id);
}

void AbstractWindow::SetWindowFrame(AbstractWindowFrame *window_frame) {
  if (window_frame == window_frame_) return;

  if (window_frame_) {
    delete window_frame_;
    window_frame_ = nullptr;
  }

  window_frame_ = window_frame;
  // TODO: check if there's original window using the window frame

  if (window_frame_) {
    window_frame_->window_ = this;
    window_frame_->window_action().Connect(this, &AbstractWindow::OnWindowAction);
    window_frame_->OnSetup();
  }
}

void AbstractWindow::Show() {
  if (!visible()) {
    shell_surface_->Commit();
  }
}

void AbstractWindow::Close(SLOT) {
  if (AbstractSurface::GetShellSurfaceCount() == 1) {
    Application::Exit();
  }

  delete this;
}

void AbstractWindow::Maximize(SLOT) {

}

void AbstractWindow::Minimize(SLOT) {

}

Size AbstractWindow::GetMinimalSize() const {
  if (IsFrameless()) return Size(100, 100);

  int w = 160, h = 120;
  switch (window_frame_->title_bar_position()) {
    case AbstractWindowFrame::kTitleBarLeft:
    case AbstractWindowFrame::kTitleBarRight: {
      w = window_frame_->title_bar_size() + window_frame_->border();
      w += 120;
      break;
    }
    case AbstractWindowFrame::kTitleBarBottom:
    case AbstractWindowFrame::kTitleBarTop:
    default: {
      h = window_frame_->title_bar_size() + window_frame_->border();
      h += 90;
      break;
    }
  }
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
    return geometry();
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
        if (mouse_task()->next()) {
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

void AbstractWindow::OnDraw(const Context *context) {
  if (window_frame_) {
    window_frame_->OnDraw(context);
  }
}

void AbstractWindow::OnMaximized(bool maximized) {

}

void AbstractWindow::OnFullscreen(bool) {

}

void AbstractWindow::OnFocus(bool focus) {
  Update();
}

void AbstractWindow::AddSubView(AbstractView *view, int pos) {
  if (view) InsertChild(view, pos);
}

void AbstractWindow::MoveWithMouse(MouseEvent *event) const {
  xdg_toplevel_.Move(event->wl_seat(), event->serial());
}

void AbstractWindow::ResizeWithMouse(MouseEvent *event, uint32_t edges) const {
  xdg_toplevel_.Resize(event->wl_seat(), event->serial(), edges);
}

void AbstractWindow::SetShellSurface(AbstractSurface *surface) {
  DBG_ASSERT(surface);
  DBG_ASSERT(nullptr == surface->parent());

  if (shell_surface_) delete shell_surface_;

  shell_surface_ = surface;

  xdg_surface_.configure().Set(this, &AbstractWindow::OnXdgSurfaceConfigure);
  xdg_surface_.Setup(Display::xdg_shell(), shell_surface_->wl_surface());

  xdg_toplevel_.configure().Set(this, &AbstractWindow::OnXdgToplevelConfigure);
  xdg_toplevel_.close().Set(this, &AbstractWindow::OnXdgToplevelClose);
  xdg_toplevel_.Setup(xdg_surface_);
}

void AbstractWindow::ResizeWindowFrame(AbstractWindowFrame *window_frame, int width, int height) {
  window_frame->OnResize(width, height);
}

void AbstractWindow::DrawWindowFrame(AbstractWindowFrame *window_frame, const Context *context) {
  window_frame->OnDraw(context);
}

void AbstractWindow::OnXdgSurfaceConfigure(uint32_t serial) {
  xdg_surface_.AckConfigure(serial);

  if (!visible()) {
    set_visible(true);
    xdg_surface_.SetWindowGeometry(shell_surface_->margin().left,
                                   shell_surface_->margin().top,
                                   width(), height());
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
    if (width == this->width() && height == this->height()) do_resize = false;
  } else {
    // Initialize
    width = this->width();
    height = this->height();
  }

  if (do_resize) {
    int x = 0, y = 0;
    x = shell_surface_->margin().left;
    y = shell_surface_->margin().top;
    xdg_surface_.SetWindowGeometry(x, y, width, height);
    resize(width, height);
    OnResize(width, height);
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
  switch (action) {
    case kActionClose: {
      Close(slot);
      break;
    }
    case kActionMaximize: {
      if (IsMaximized()) {
        xdg_toplevel_.UnsetMaximized();
      } else {
        xdg_toplevel_.SetMaximized();
      }
      break;
    }
    case kActionMinimize: {
      Bit::Set<int>(flags_, kFlagMaskMinimized);
      xdg_toplevel_.SetMinimized();
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

}
