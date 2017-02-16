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
#include <skland/gui/output.hpp>
#include <skland/gui/context.hpp>
#include <skland/gui/abstract-window-frame.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/sub-surface.hpp>

#include <skland/graphic/canvas.hpp>

#include "internal/view-task.hpp"
#include "internal/redraw-task.hpp"

namespace skland {

Window::Window(const char *title, AbstractWindowFrame *frame)
    : Window(640, 480, title, frame) {
}

Window::Window(int width, int height, const char *title, AbstractWindowFrame *frame)
    : AbstractWindow(width, height, title, frame),
      main_surface_(nullptr),
      main_widget_(nullptr) {
  if (frame) {
    ViewSurface *parent = toplevel_shell_surface()->view_surface();
    main_surface_ = new SubSurface(parent, this, Theme::shadow_margin());
    DBG_ASSERT(main_surface_->view_surface()->parent() == parent);
    DBG_ASSERT(main_surface_->view_surface()->below() == parent);
    wayland::Region empty_region;
    empty_region.Setup(Display::wl_compositor());
    main_surface_->view_surface()->SetInputRegion(empty_region);
  }
}

Window::~Window() {
  delete main_widget_;
  delete main_surface_;
}

void Window::SetMainWidget(AbstractWidget *widget) {
  if (widget == main_widget_) return;

  if (main_widget_) delete main_widget_;
  main_widget_ = widget;

  AddSubView(widget);
  SetMainWidgetGeometry();
}

void Window::OnShown() {
  ViewSurface *shell_surface = toplevel_shell_surface()->view_surface();

  // Create buffer:
  Size output_size(1024, 800);
  if (const Output *output = Display::GetOutputAt(0)) {
    output_size = output->current_mode_size();  // The current screen size
  }

  int total_width = std::max(this->width(), output_size.width);
  int total_height = std::max(this->height(), output_size.height);
  total_width += shell_surface->margin().lr();
  total_height += shell_surface->margin().tb();

  frame_pool_.Setup(total_width * 4 * total_height);
  frame_buffer_.Setup(frame_pool_, total_width, total_height,
                      total_width * 4, WL_SHM_FORMAT_ARGB8888);

  shell_surface->Attach(&frame_buffer_);
  frame_canvas_.reset(new Canvas((unsigned char *) frame_buffer_.pixel(),
                                 frame_buffer_.size().width,
                                 frame_buffer_.size().height));
  frame_canvas_->SetOrigin((float) shell_surface->margin().left,
                           (float) shell_surface->margin().top);
  frame_canvas_->Clear();

  if (main_surface_) {
    main_pool_.Setup(total_width * 4 * total_height);
    main_buffer_.Setup(main_pool_, total_width, total_height,
                       total_width * 4, WL_SHM_FORMAT_ARGB8888);
    main_surface_->view_surface()->Attach(&main_buffer_);
    main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.pixel(),
                                  main_buffer_.size().width,
                                  main_buffer_.size().height));
    main_canvas_->SetOrigin((float) main_surface_->view_surface()->margin().left,
                            (float) main_surface_->view_surface()->margin().top);
    main_canvas_->Clear();
  }

  UpdateAll();
}

void Window::OnUpdate(AbstractView *view) {
  if (!visible()) return;

  ViewSurface *surface = nullptr;

  if (view == this) {
    surface = toplevel_shell_surface()->view_surface();
    kRedrawTaskTail.PushFront(redraw_task().get());
    redraw_task()->context = Context(surface, frame_canvas_);
    DBG_ASSERT(frame_canvas_);
    Damage(this, 0, 0,
           width() + surface->margin().lr(),
           height() + surface->margin().tb());
    surface->Commit();
  } else {
    std::shared_ptr<Canvas> canvas;
    if (main_surface_) {
      surface = main_surface_->view_surface();
      canvas = main_canvas_;
    } else {
      surface = toplevel_shell_surface()->view_surface();
      canvas = frame_canvas_;
    }

    RedrawTask *task = GetRedrawTask(view);
    kRedrawTaskTail.PushFront(task);
    task->context = Context(surface, canvas);
    DBG_ASSERT(canvas);
    Damage(view, view->x() + surface->margin().left,
           view->y() + surface->margin().top,
           view->width(),
           view->height());
    surface->Commit();
  }
}

ViewSurface *Window::OnGetSurface(const AbstractView *view) const {
  if (view == this)
    return toplevel_shell_surface()->view_surface();

  return nullptr != main_surface_ ? main_surface_->view_surface() : toplevel_shell_surface()->view_surface();
}

void Window::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void Window::OnResize(int width, int height) {
  RectI input_rect(width, height);
  ViewSurface *shell_surface = toplevel_shell_surface()->view_surface();

  input_rect.left = shell_surface->margin().left - AbstractWindowFrame::kResizingMargin.left;
  input_rect.top = shell_surface->margin().top - AbstractWindowFrame::kResizingMargin.top;
  input_rect.Resize(width + AbstractWindowFrame::kResizingMargin.lr(),
                    height + AbstractWindowFrame::kResizingMargin.tb());

  wayland::Region input_region;
  input_region.Setup(Display::wl_compositor());
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  // Reset buffer:
  width += shell_surface->margin().lr();
  height += shell_surface->margin().tb();

  int total_size = width * 4 * height;
  if (total_size > frame_pool_.size()) {
    DBG_PRINT_MSG("size_required: %d, pool size: %d, %s\n",
                  total_size, frame_pool_.size(), "Re-generate shm pool");
    frame_pool_.Setup(total_size);
  }

  frame_buffer_.Setup(frame_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&frame_buffer_);
  frame_canvas_.reset(new Canvas((unsigned char *) frame_buffer_.pixel(),
                                 frame_buffer_.size().width,
                                 frame_buffer_.size().height));
  frame_canvas_->SetOrigin(shell_surface->margin().left, shell_surface->margin().top);
  frame_canvas_->Clear();

  if (main_surface_) {
    main_pool_.Setup(total_size);
    main_buffer_.Setup(main_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
    main_surface_->view_surface()->Attach(&main_buffer_);
    main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.pixel(),
                                  main_buffer_.size().width,
                                  main_buffer_.size().height));
    main_canvas_->SetOrigin(main_surface_->view_surface()->margin().left,
                            main_surface_->view_surface()->margin().top);
    main_canvas_->Clear();

    DBG_ASSERT(window_frame());
    ResizeWindowFrame(window_frame(), width, height);
  }

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
