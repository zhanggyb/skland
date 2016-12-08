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

#include <skland/gui/window.hpp>

#include <skland/core/types.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/output.hpp>
#include <skland/gui/surface.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/gui/window-frame.hpp>

namespace skland {

Window::Window(const char *title, int flags)
    : Window(640, 480, title, flags) {
}

Window::Window(int width, int height, const char *title, int flags)
    : AbstractWindow(width, height, title, flags),
      is_configured_(false),
      maximized_(false),
      minimized_(false),
      fullscreened_(false),
      window_frame_(nullptr),
      main_widget_(nullptr) {

  set_name(title);  // debug only

  Surface *surface = new Surface(this, AbstractWindowFrame::kShadowMargin);
  SetSurface(surface);

  xdg_surface_.configure().Set(this, &Window::OnXdgSurfaceConfigure);
  xdg_surface_.Setup(Display::xdg_shell(), surface->wl_surface());

  xdg_toplevel_.configure().Set(this, &Window::OnXdgToplevelConfigure);
  xdg_toplevel_.close().Set(this, &Window::OnXdgToplevelClose);
  xdg_toplevel_.Setup(xdg_surface_);

  Size output_size(1024, 800);
  if (const Output *output = Display::GetOutputAt(0)) {
    output_size = output->current_mode_size();  // The current screen size
  }

  int total_width = std::max(this->width(), output_size.width);
  int total_height = std::max(this->height(), output_size.height);

  int x = 0, y = 0, w = this->width(), h = this->height();  // The input region

  if (flags & kWindowFramelessMask) {
    window_frame_ = new WindowFrame(this, 0, AbstractWindowFrame::kTitleBarNone);
  } else {
    window_frame_ = new WindowFrame(this, 5);
    total_width += window_frame_->kShadowMargin.lr();
    total_height += window_frame_->kShadowMargin.tb();
    x += window_frame_->kShadowMargin.left - window_frame_->kResizingMargin.left;
    y += window_frame_->kShadowMargin.top - window_frame_->kResizingMargin.top;
    w += window_frame_->kResizingMargin.lr();
    h += window_frame_->kResizingMargin.tb();
  }

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(x, y, w, h);
  surface->SetInputRegion(input_region_);

  pool_.Setup(total_width * 4 * total_height);
  buffer_.Setup(pool_, total_width, total_height, total_width * 4, WL_SHM_FORMAT_ARGB8888);
}

Window::~Window() {
  delete main_widget_;
  delete window_frame_;
}

void Window::SetMainWidget(AbstractWidget *widget) {
  if (main_widget_) {
    delete main_widget_;
  }
  main_widget_ = widget;

  AddSubView(widget, -1);
  SetMainWidgetGeometry();
}

Size Window::GetMinimalSize() const {
  if (window_frame_ == nullptr)
    return Size(100, 100);

  int w = 160, h = 160;
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

void Window::OnMouseEnter(MouseEvent *event) {
  int location = window_frame_->GetPointerLocation(event);
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
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }
  event->Accept();
}

void Window::OnMouseLeave(MouseEvent *event) {
  event->Accept();
}

void Window::OnMouseMove(MouseEvent *event) {

  int location = window_frame_->GetPointerLocation(event);
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
    default: {
      event->SetCursor(Display::cursor(kCursorLeftPtr));
      break;
    }
  }

  event->Accept();
}

void Window::OnMouseButton(MouseEvent *event) {
  if ((event->button() == kMouseButtonLeft) &&
      (event->state() == kMouseButtonPressed)) {

    int location = window_frame_->GetPointerLocation(event);

    if (location == kTitleBar) {
      if (HaveNextMouseTask()) {
        event->Accept();
        return;
      }

      xdg_toplevel_.Move(event->wl_seat(), event->serial());
      event->Ignore();
      return;
    }

    if (location < kResizeMask) {
      xdg_toplevel_.Resize(event->wl_seat(), event->serial(), (uint32_t) location);
      event->Ignore();
      return;
    }
  }

  event->Accept();
}

void Window::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }

  event->Accept();
}

void Window::OnResize(int width, int height) {
  resize(width, height);

  int total_width = this->width();
  int total_height = this->height();
  Rect input_rect(width, height);
  int x = 0, y = 0;

  if (window_frame_) {
    total_width += window_frame_->kShadowMargin.lr();
    total_height += window_frame_->kShadowMargin.tb();
    input_rect.left = window_frame_->kShadowMargin.left - window_frame_->kResizingMargin.left;
    input_rect.top = window_frame_->kShadowMargin.top - window_frame_->kResizingMargin.top;
    input_rect.Resize(width + window_frame_->kResizingMargin.lr(),
                      height + window_frame_->kResizingMargin.tb());
    x = AbstractWindowFrame::kShadowMargin.left;
    y = AbstractWindowFrame::kShadowMargin.top;
    window_frame_->Resize(width, height);
  }

  int total_size = total_width * 4 * total_height;
  if (total_size > pool_.size()) {
    DBG_PRINT_MSG("size_required: %d, pool size: %d, %s\n", total_size, pool_.size(), "Re-generate shm pool");
    pool_.Setup(total_size);
  }
  buffer_.Setup(pool_, total_width, total_height, total_width * 4, WL_SHM_FORMAT_ARGB8888);
  surface()->Attach(buffer_);

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(input_rect.x(), input_rect.y(), input_rect.width(), input_rect.height());
  surface()->SetInputRegion(input_region_);

  surface()->Damage(0, 0, total_width, total_height);

  xdg_surface_.SetWindowGeometry(x, y, width, height);

  RedrawAll();

  SetMainWidgetGeometry();
}

void Window::OnDraw(Canvas *canvas) {
  window_frame_->Draw(canvas);
}

void Window::OnXdgSurfaceConfigure(uint32_t serial) {
  xdg_surface_.AckConfigure(serial);

  if (!is_configured_) {
    int x = AbstractWindowFrame::kShadowMargin.left;
    int y = AbstractWindowFrame::kShadowMargin.top;
    int w = width();
    int h = height();

    xdg_surface_.SetWindowGeometry(x, y, w, h);

    surface()->Attach(buffer_);
    RedrawAll();
    is_configured_ = true;
  }
}

void Window::OnXdgToplevelConfigure(int width, int height, int states) {
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
    if (width != this->width() || height != this->height())
      OnResize(width, height);
  }
}

void Window::OnXdgToplevelClose() {
  DBG_PRINT_MSG("%s\n", "toplevel close");
  // TODO: close window
}

void Window::SetMainWidgetGeometry() {
  if (main_widget_ == nullptr) return;

  Rect rect(width(), height());

  if (window_frame_) {
    rect = window_frame_->GetLocalClientGeometry();
  }

  main_widget_->SetPosition(rect.x(), rect.y());
  main_widget_->Resize(rect.width(), rect.height());
}

}
