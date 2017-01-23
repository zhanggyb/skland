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
#include <skland/gui/shm-surface.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-window-frame.hpp>
#include <skland/gui/output.hpp>

#include <skland/graphic/canvas.hpp>

#include "internal/view-task.hpp"
#include "internal/redraw-task.hpp"

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
      window_frame_(nullptr),
      flags_(0),
      is_xdg_surface_configured_(false),
      frame_surface_(nullptr),
      main_surface_(nullptr) {
  if (title) title_ = title;

  int x = 0, y = 0;  // The input region
  AbstractSurface *shell_surface = nullptr;

  if (frame) {
    frame_surface_ = new ShmSurface(this, Theme::shadow_margin());
    main_surface_ = new ShmSurface(this, Theme::shadow_margin());

    // There's a known issue in current gnome that a sub surface cannot be placed below its parent
    frame_surface_->AddSubSurface(main_surface_);

    SetWindowFrame(frame);
    x += frame_surface_->margin().left - AbstractWindowFrame::kResizingMargin.left;
    y += frame_surface_->margin().top - AbstractWindowFrame::kResizingMargin.top;
    width += AbstractWindowFrame::kResizingMargin.lr();
    height += AbstractWindowFrame::kResizingMargin.tb();

    inactive_region_.Setup(Display::wl_compositor());
    inactive_region_.Add(0, 0, 0, 0);
    main_surface_->SetInputRegion(inactive_region_);
    shell_surface = frame_surface_;
  } else {
    main_surface_ = new ShmSurface(this);
    shell_surface = main_surface_;
  }

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(x, y, width, height);
  shell_surface->SetInputRegion(input_region_);

  xdg_surface_.configure().Set(this, &AbstractWindow::OnXdgSurfaceConfigure);
  xdg_surface_.Setup(Display::xdg_shell(), shell_surface->wl_surface());

  xdg_toplevel_.configure().Set(this, &AbstractWindow::OnXdgToplevelConfigure);
  xdg_toplevel_.close().Set(this, &AbstractWindow::OnXdgToplevelClose);
  xdg_toplevel_.Setup(xdg_surface_);
  xdg_toplevel_.SetTitle(title_.c_str()); // TODO: support multi-language

  set_name(title);  // debug only

  // Create buffer:
  Size output_size(1024, 800);
  if (const Output *output = Display::GetOutputAt(0)) {
    output_size = output->current_mode_size();  // The current screen size
  }

  int total_width = std::max((int) geometry().width(), output_size.width);
  int total_height = std::max((int) geometry().height(), output_size.height);
  total_width += shell_surface->margin().lr();
  total_height += shell_surface->margin().tb();

  main_pool_.Setup(total_width * 4 * total_height);
  main_buffer_.Setup(main_pool_, total_width, total_height,
                     total_width * 4, WL_SHM_FORMAT_ARGB8888);

  frame_pool_.Setup(total_width * 4 * total_height);
  frame_buffer_.Setup(frame_pool_, total_width, total_height,
                      total_width * 4, WL_SHM_FORMAT_ARGB8888);
}

AbstractWindow::~AbstractWindow() {
  delete window_frame_;
  delete main_surface_;
  delete frame_surface_;
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
  if (!is_xdg_surface_configured_) {
    main_surface_->Commit();
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

void AbstractWindow::OnUpdate(AbstractView *view) {
  if (!is_xdg_surface_configured_) return;

  if (view == this) {
    if (frame_surface_) {
      kRedrawTaskTail.PushFront(redraw_task().get());
      // TODO: this is just a workaround, should use frame_surface for frame and background,
      // but now use main_surface instead.
      redraw_task()->context = frame_surface_;
      DBG_ASSERT(redraw_task()->context.GetCanvas());
      frame_surface_->Damage((int) geometry().x() + frame_surface_->margin().left,
                             (int) geometry().y() + frame_surface_->margin().top,
                             (int) geometry().width(),
                             (int) geometry().height());
      frame_surface_->Commit();
    }
  } else {
    kRedrawTaskTail.PushFront(view->redraw_task().get());

    // TODO: this is juat a workaround, should render widgets on main_surface
    view->redraw_task()->context = main_surface_;
    DBG_ASSERT(view->redraw_task()->context.GetCanvas());
    main_surface_->Damage((int) view->geometry().x() + main_surface_->margin().left,
                          (int) view->geometry().y() + main_surface_->margin().top,
                          (int) view->geometry().width(),
                          (int) view->geometry().height());
    main_surface_->Commit();
  }
}

AbstractSurface *AbstractWindow::OnGetSurface(const AbstractView *view) const {
  return main_surface_;
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

void AbstractWindow::OnDraw(const Context *context) {
  if (window_frame_) {
    window_frame_->OnDraw(context);
  }
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

  if (!is_xdg_surface_configured_) {
    is_xdg_surface_configured_ = true;

    int x = main_surface_->margin().left;
    int y = main_surface_->margin().top;
    int w = (int) geometry().width();
    int h = (int) geometry().height();
    xdg_surface_.SetWindowGeometry(x, y, w, h);

    if (frame_surface_) {
      frame_surface_->Attach(&frame_buffer_);
      frame_surface_->GetCanvas()->Clear();
    }

    main_surface_->Attach(&main_buffer_);
    main_surface_->GetCanvas()->Clear();
    UpdateAll();
  }
}

void AbstractWindow::OnXdgToplevelConfigure(int width, int height, int states) {
  using wayland::XdgToplevel;

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
    if (width != geometry().width() || height != geometry().height()) {

      Rect input_rect(width, height);
      int x = 0, y = 0;
      AbstractSurface *shell_surface = frame_surface_ ? frame_surface_ : main_surface_;

      if (!IsFrameless()) {
        input_rect.left = frame_surface_->margin().left - AbstractWindowFrame::kResizingMargin.left;
        input_rect.top = frame_surface_->margin().top - AbstractWindowFrame::kResizingMargin.top;
        input_rect.Resize(width + AbstractWindowFrame::kResizingMargin.lr(),
                          height + AbstractWindowFrame::kResizingMargin.tb());
        x = frame_surface_->margin().left;
        y = frame_surface_->margin().top;
        window_frame_->OnResize(width, height);
      }

      input_region_.Setup(Display::wl_compositor());
      input_region_.Add((int) input_rect.x(),
                        (int) input_rect.y(),
                        (int) input_rect.width(),
                        (int) input_rect.height());
      shell_surface->SetInputRegion(input_region_);

      xdg_surface_.SetWindowGeometry(x, y, width, height);

      resize(width, height);

      // Reset buffer:
      width += shell_surface->margin().lr();
      height += shell_surface->margin().tb();

      int total_size = width * 4 * height;
      if (total_size > main_pool_.size()) {
        DBG_PRINT_MSG("size_required: %d, pool size: %d, %s\n", total_size, main_pool_.size(), "Re-generate shm pool");
        main_pool_.Setup(total_size);
      }
      main_buffer_.Setup(main_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
      main_surface_->Attach(&main_buffer_);

      if (frame_surface_) {
        frame_buffer_.Setup(frame_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
        frame_surface_->Attach(&frame_buffer_);
      }

      main_surface_->GetCanvas()->Clear();
      UpdateAll();

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
