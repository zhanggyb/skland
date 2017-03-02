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
#include <skland/gui/context.hpp>
#include <skland/gui/abstract-window-frame.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/sub-surface.hpp>

#include <skland/graphic/canvas.hpp>

#include "internal/display-proxy.hpp"
#include "internal/redraw-task.hpp"
#include "internal/redraw-task-proxy.hpp"

namespace skland {

Window::Window(const char *title, AbstractWindowFrame *frame)
    : Window(640, 480, title, frame) {
}

Window::Window(int width, int height, const char *title, AbstractWindowFrame *frame)
    : AbstractWindow(width, height, title, nullptr, frame),
      main_surface_(nullptr) {
  if (frame) {
    Surface *parent = shell_surface();
    main_surface_ = SubSurface::Create(parent, this, Theme::shadow_margin());
    DBG_ASSERT(main_surface_->parent() == parent);
    DBG_ASSERT(main_surface_->below() == parent);
    wayland::Region empty_region;
    empty_region.Setup(DisplayProxy().wl_compositor());
    main_surface_->SetInputRegion(empty_region);
  }
}

Window::~Window() {
  delete main_surface_;
}

void Window::OnShown() {
  Surface *shell_surface = this->shell_surface();

  // Create buffer:
  int total_width = size().width;
  int total_height = size().height;
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
    main_surface_->Attach(&main_buffer_);
    main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.pixel(),
                                  main_buffer_.size().width,
                                  main_buffer_.size().height));
    main_canvas_->SetOrigin((float) main_surface_->margin().left,
                            (float) main_surface_->margin().top);
    main_canvas_->Clear();
  }

  OnUpdate(nullptr);
  if (content_view()) UpdateAll(content_view());
}

void Window::OnUpdate(AbstractView *view) {
  if (!shown()) return;

  Surface *surface = nullptr;

  if (nullptr == view) {
    surface = this->shell_surface();
    RedrawTaskProxy redraw_task_helper(this);
    redraw_task_helper.MoveToTail();
    redraw_task_helper.SetContext(Context(surface, frame_canvas_));
    DBG_ASSERT(frame_canvas_);
    Damage(this, 0, 0,
           size().width + surface->margin().lr(),
           size().height + surface->margin().tb());
    surface->Commit();
  } else {
    std::shared_ptr<Canvas> canvas;
    if (main_surface_) {
      surface = main_surface_;
      canvas = main_canvas_;
    } else {
      surface = this->shell_surface();
      canvas = frame_canvas_;
    }

    RedrawTaskProxy redraw_task_helper(view);
    redraw_task_helper.MoveToTail();
    redraw_task_helper.SetContext(Context(surface, canvas));
    DBG_ASSERT(canvas);
    Damage(view, view->x() + surface->margin().left,
           view->y() + surface->margin().top,
           view->width(),
           view->height());
    surface->Commit();
  }
}

Surface *Window::GetSurface(const AbstractView *view) const {
  if (nullptr == view)
    return shell_surface();

  return nullptr != main_surface_ ? main_surface_ : shell_surface();
}

void Window::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void Window::OnSizeChanged(int width, int height) {
  RectI input_rect(width, height);
  Surface *shell_surface = this->shell_surface();

  input_rect.left = shell_surface->margin().left - AbstractWindowFrame::kResizingMargin.left;
  input_rect.top = shell_surface->margin().top - AbstractWindowFrame::kResizingMargin.top;
  input_rect.Resize(width + AbstractWindowFrame::kResizingMargin.lr(),
                    height + AbstractWindowFrame::kResizingMargin.tb());

  wayland::Region input_region;
  input_region.Setup(DisplayProxy().wl_compositor());
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  // Reset buffer:
  width += shell_surface->margin().lr();
  height += shell_surface->margin().tb();

  int total_size = width * 4 * height;
  frame_pool_.Setup(total_size);

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
    main_surface_->Attach(&main_buffer_);
    main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.pixel(),
                                  main_buffer_.size().width,
                                  main_buffer_.size().height));
    main_canvas_->SetOrigin(main_surface_->margin().left,
                            main_surface_->margin().top);
    main_canvas_->Clear();
  }

  OnUpdate(nullptr);
  if (content_view()) UpdateAll(content_view());
}

}
