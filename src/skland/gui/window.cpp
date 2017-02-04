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

#include <skland/gui/application.hpp>
#include <skland/gui/abstract-widget.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/shm-surface.hpp>

#include <skland/gui/output.hpp>

#include <skland/gui/context.hpp>
#include <skland/graphic/canvas.hpp>

#include <skland/wayland/region.hpp>

#include "internal/view-task.hpp"
#include "internal/redraw-task.hpp"

namespace skland {

Window::Window(const char *title, AbstractWindowFrame *frame)
    : Window(640, 480, title, frame) {
}

Window::Window(int width, int height, const char *title, AbstractWindowFrame *frame)
    : AbstractWindow(width, height, title, frame),
      frame_surface_(nullptr),
      main_surface_(nullptr),
      main_widget_(nullptr) {
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

    wayland::Region empty_region;
    empty_region.Setup(Display::wl_compositor());
    main_surface_->SetInputRegion(empty_region);

    shell_surface = frame_surface_;
  } else {
    main_surface_ = new ShmSurface(this);
    shell_surface = main_surface_;
  }

  wayland::Region input_region;
  input_region.Setup(Display::wl_compositor());
  input_region.Add(x, y, width, height);
  shell_surface->SetInputRegion(input_region);

  SetShellSurface(shell_surface);
  SetTitle(title); // TODO: support multi-language

  // Create buffer:
  Size output_size(1024, 800);
  if (const Output *output = Display::GetOutputAt(0)) {
    output_size = output->current_mode_size();  // The current screen size
  }

  int total_width = std::max(this->width(), output_size.width);
  int total_height = std::max(this->height(), output_size.height);
  total_width += shell_surface->margin().lr();
  total_height += shell_surface->margin().tb();

  main_pool_.Setup(total_width * 4 * total_height);
  main_buffer_.Setup(main_pool_, total_width, total_height,
                     total_width * 4, WL_SHM_FORMAT_ARGB8888);

  if (frame) {
    frame_pool_.Setup(total_width * 4 * total_height);
    frame_buffer_.Setup(frame_pool_, total_width, total_height,
                        total_width * 4, WL_SHM_FORMAT_ARGB8888);
  }
}

Window::~Window() {
//  delete main_surface_;
//  delete frame_surface_;
  delete main_widget_;
}

void Window::SetMainWidget(AbstractWidget *widget) {
  if (widget == main_widget_) return;

  if (main_widget_) delete main_widget_;
  main_widget_ = widget;

  AddSubView(widget);
  SetMainWidgetGeometry();
}

void Window::OnShown() {
  if (frame_surface_) {
    frame_surface_->Attach(&frame_buffer_);
    frame_surface_->GetCanvas()->Clear();
  }

  main_surface_->Attach(&main_buffer_);
  main_surface_->GetCanvas()->Clear();
  UpdateAll();
}

void Window::OnUpdate(AbstractView *view) {
  if (!visible()) return;

  if (view == this) {
    if (frame_surface_) {
      kRedrawTaskTail.PushFront(redraw_task().get());
      // TODO: this is just a workaround, should use frame_surface for frame and background,
      // but now use main_surface instead.
      redraw_task()->context = frame_surface_;
      DBG_ASSERT(redraw_task()->context.GetCanvas());
      frame_surface_->Damage(0, 0,
                             width() + frame_surface_->margin().lr(),
                             height() + frame_surface_->margin().tb());
      frame_surface_->Commit();
    }
  } else {
    kRedrawTaskTail.PushFront(GetRedrawTask(view));

    // TODO: this is juat a workaround, should render widgets on main_surface
    GetRedrawTask(view)->context = main_surface_;
    DBG_ASSERT(GetRedrawTask(view)->context.GetCanvas());
    main_surface_->Damage(view->x() + main_surface_->margin().left,
                          view->y() + main_surface_->margin().top,
                          view->width(),
                          view->height());
    main_surface_->Commit();
  }
}

AbstractSurface *Window::OnGetSurface(const AbstractView *view) const {
  return main_surface_;
}

void Window::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void Window::OnResize(int width, int height) {
  Rect input_rect(width, height);
  AbstractSurface *shell_surface = frame_surface_ ? frame_surface_ : main_surface_;

  if (!IsFrameless()) {
    input_rect.left = frame_surface_->margin().left - AbstractWindowFrame::kResizingMargin.left;
    input_rect.top = frame_surface_->margin().top - AbstractWindowFrame::kResizingMargin.top;
    input_rect.Resize(width + AbstractWindowFrame::kResizingMargin.lr(),
                      height + AbstractWindowFrame::kResizingMargin.tb());
    ResizeWindowFrame(window_frame(), width, height);
  }

  wayland::Region input_region;
  input_region.Setup(Display::wl_compositor());
  input_region.Add((int) input_rect.x(),
                   (int) input_rect.y(),
                   (int) input_rect.width(),
                   (int) input_rect.height());
  shell_surface->SetInputRegion(input_region);

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

  SetMainWidgetGeometry();
  UpdateAll();
}

void Window::SetMainWidgetGeometry() {
  if (main_widget_ == nullptr) return;

  Rect rect = GetClientGeometry();

  main_widget_->MoveTo((int) rect.x(), (int) rect.y());
  main_widget_->Resize((int) rect.width(), (int) rect.height());
}

}
