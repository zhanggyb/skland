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
#include <skland/gui/surface.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-window-frame.hpp>

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
      display_(nullptr),
      window_frame_(nullptr),
      flags_(0) {
  if (title) title_ = title;

  int x = 0, y = 0;  // The input region

  Surface *surface = nullptr;

  if (frame) {
    surface = new Surface(this, Margin(Theme::shadow_margin_left(),
                                       Theme::shadow_margin_top(),
                                       Theme::shadow_margin_right(),
                                       Theme::shadow_margin_bottom()));
    SetWindowFrame(frame);
    x += surface->margin().left - AbstractWindowFrame::kResizingMargin.left;
    y += surface->margin().top - AbstractWindowFrame::kResizingMargin.top;
    width += AbstractWindowFrame::kResizingMargin.lr();
    height += AbstractWindowFrame::kResizingMargin.tb();
  } else {
    surface = new Surface(this);
  }

  SetSurface(surface);
  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(x, y, width, height);
  surface->SetInputRegion(input_region_);

  xdg_surface_.configure().Set(this, &AbstractWindow::OnXdgSurfaceConfigure);
  xdg_surface_.Setup(Display::xdg_shell(), surface->wl_surface());

  xdg_toplevel_.configure().Set(this, &AbstractWindow::OnXdgToplevelConfigure);
  xdg_toplevel_.close().Set(this, &AbstractWindow::OnXdgToplevelClose);
  xdg_toplevel_.Setup(xdg_surface_);
  xdg_toplevel_.SetTitle(title_.c_str());
  xdg_toplevel_.SetAppId(title_.c_str());

  if (title) xdg_toplevel_.SetTitle(title);

  Display::AddWindow(this);
  // TODO: layout in display
}

AbstractWindow::~AbstractWindow() {
  if (display_)
    RemoveManagedObject(display_,
                        this,
                        &display_,
                        &display_->first_window_,
                        &display_->last_window_,
                        display_->windows_count_);
  DBG_ASSERT(display_ == nullptr);

  delete window_frame_;
}

void AbstractWindow::SetTitle(const char *title) {
  title_ = title;
  xdg_toplevel_.SetTitle(title);
}

void AbstractWindow::SetWindowFrame(AbstractWindowFrame *window_frame) {
  if (window_frame == window_frame_) return;

  if (window_frame_) {
    delete window_frame_;
    window_frame_ = nullptr;
  }

  if (window_frame) {
    // TODO: check the original window use the window frame

    window_frame_ = window_frame;
    window_frame_->window_ = this;
    window_frame_->window_action().Connect(this, &AbstractWindow::OnWindowAction);
    window_frame_->OnSetup();
  }
}

void AbstractWindow::Close(SLOT) {
  if (Display::windows_count() == 1) {
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
  if (IsFrameless()) {
    DBG_ASSERT(nullptr == window_frame_);
    return kInterior;
  }

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
        if (mouse_task().next()) {
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

void AbstractWindow::OnDraw(Canvas *canvas) {
  if (window_frame_) window_frame_->OnDraw(canvas);
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

void AbstractWindow::OnXdgSurfaceConfigure(uint32_t serial) {
  xdg_surface_.AckConfigure(serial);

  if (nullptr == surface()->canvas()) {
    int x = surface()->margin().left;
    int y = surface()->margin().top;
    int w = (int) width();
    int h = (int) height();

    xdg_surface_.SetWindowGeometry(x, y, w, h);
    OnConfigureCanvas();
  }
}

void AbstractWindow::OnXdgToplevelConfigure(int width, int height, int states) {
  using wayland::client::XdgToplevel;

  bool maximized = ((states & XdgToplevel::kStateMaskMaximized) != 0);
  bool fullscreen = ((states & XdgToplevel::kStateMaskFullscreen) != 0);
  bool resizing = ((states & XdgToplevel::kStateMaskResizing) != 0);
//  bool focus = ((states & XdgToplevel::kStateMaskActivated) != 0);

  Size min = this->GetMinimalSize();
  Size max = this->GetMaximalSize();
  DBG_ASSERT(min.width < max.width && min.height < max.height);

  width = clamp(width, min.width, max.width);
  height = clamp(height, min.height, max.height);

  if (maximized || fullscreen || resizing) {
    if (width != this->width() || height != this->height()) {

      Rect input_rect(width, height);
      int x = 0, y = 0;

      if (!IsFrameless()) {
        input_rect.left = surface()->margin().left - AbstractWindowFrame::kResizingMargin.left;
        input_rect.top = surface()->margin().top - AbstractWindowFrame::kResizingMargin.top;
        input_rect.Resize(width + AbstractWindowFrame::kResizingMargin.lr(),
                          height + AbstractWindowFrame::kResizingMargin.tb());
        x = surface()->margin().left;
        y = surface()->margin().top;
      }

      input_region_.Setup(Display::wl_compositor());
      input_region_.Add((int) input_rect.x(),
                        (int) input_rect.y(),
                        (int) input_rect.width(),
                        (int) input_rect.height());
      surface()->SetInputRegion(input_region_);

      xdg_surface_.SetWindowGeometry(x, y, width, height);

      if (!IsFrameless()) window_frame_->OnResize(width, height);
      OnResize(width, height);
    }
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
      // TODO: maximize
      if (IsMaximized()) {
        clear_bit<int>(flags_, 0x3);
        xdg_toplevel_.UnsetMaximized();
      } else {
        clear_bit<int>(flags_, 0x3);
        set_bit<int>(flags_, kWindowMaximized);
        xdg_toplevel_.SetMaximized();
      }
      break;
    }
    case kActionMinimize: {
      fprintf(stderr, "minimize\n");
      // TODO: minimize
      xdg_toplevel_.SetMinimized();
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
