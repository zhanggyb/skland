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

#include <skland/gui/egl-window.hpp>

#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/shm-surface.hpp>
#include <skland/gui/output.hpp>
#include <skland/gui/abstract-window-frame.hpp>

#include <skland/graphic/canvas.hpp>

#include "internal/redraw-task.hpp"

#include <GLES2/gl2.h>

namespace skland {

EGLWindow::EGLWindow(const char *title, AbstractWindowFrame *frame)
    : EGLWindow(400, 300, title, frame) {

}

EGLWindow::EGLWindow(int width, int height, const char *title, AbstractWindowFrame *frame)
    : AbstractWindow(width, height, title, frame),
      frame_surface_(nullptr),
      main_surface_(nullptr),
      surface_(nullptr),
      animating_(false) {
  int x = 0, y = 0; // The input region
  AbstractSurface *shell_surface = nullptr;
  wayland::Region input_region, empty_region;

  if (frame) {
    SetWindowFrame(frame);

    frame_surface_ = new ShmSurface(this, Theme::shadow_margin());
    main_surface_ = new ShmSurface(this, Theme::shadow_margin());
    surface_ = new EGLSurface(this);

    frame_surface_->AddSubSurface(main_surface_);
    main_surface_->AddSubSurface(surface_);

    Rect client_rect = frame->GetClientGeometry();
    surface_->SetWindowPosition((int) client_rect.l,
                                (int) client_rect.t);
    surface_->Resize((int) client_rect.width(), (int) client_rect.height());
    surface_->SetDesync();  // FIXME: looks no difference between sync and desync modes

    x += frame_surface_->margin().left - AbstractWindowFrame::kResizingMargin.left;
    y += frame_surface_->margin().top - AbstractWindowFrame::kResizingMargin.top;
    width += AbstractWindowFrame::kResizingMargin.lr();
    height += AbstractWindowFrame::kResizingMargin.tb();

    input_region.Setup(Display::wl_compositor());
    input_region.Add(x, y, width, height);
    frame_surface_->SetInputRegion(input_region);

    // Create buffer:
    Size output_size(1024, 800);
    if (const Output *output = Display::GetOutputAt(0)) {
      output_size = output->current_mode_size();  // The current screen size
    }

    int total_width = std::max(this->width(), output_size.width);
    int total_height = std::max(this->height(), output_size.height);
    total_width += frame_surface_->margin().lr();
    total_height += frame_surface_->margin().tb();

    frame_pool_.Setup(total_width * 4 * total_height);
    frame_buffer_.Setup(frame_pool_, total_width, total_height,
                        total_width * 4, WL_SHM_FORMAT_ARGB8888);
//    frame_buffer_.release().Set(this, &EGLWindow::OnRelease);

    main_pool_.Setup(total_width * 4 * total_height);
    main_buffer_.Setup(main_pool_, total_width, total_height,
                       total_width * 4, WL_SHM_FORMAT_ARGB8888);

    empty_region.Setup(Display::wl_compositor());
    main_surface_->SetInputRegion(empty_region);

    empty_region.Setup(Display::wl_compositor());
    surface_->SetInputRegion(empty_region);

    shell_surface = frame_surface_;
  } else {
    surface_ = new EGLSurface(this);
    input_region.Setup(Display::wl_compositor());
    input_region.Add(0, 0, this->width(), this->height());
    surface_->SetInputRegion(input_region);
    shell_surface = surface_;
  }

//  SetToplevelShellSurface(shell_surface);
  SetTitle(title);

  frame_callback_.done().Set(this, &EGLWindow::OnFrame);
}

EGLWindow::~EGLWindow() {
  // shell surface is deleted in AbstractWindow
}

void EGLWindow::OnShown() {
  if (frame_surface_) {
    frame_surface_->Attach(&frame_buffer_);
    frame_surface_->GetCanvas()->Clear();
    main_surface_->Attach(&main_buffer_);
    main_surface_->GetCanvas()->Clear();
  }

  UpdateAll();
}

void EGLWindow::OnUpdate(AbstractView *view) {
  if (!visible()) return;

//  if (frame_surface_) {
//    if (view == this) {
//      kRedrawTaskTail.PushFront(redraw_task().get());
//      redraw_task()->context = frame_surface_;
//      DBG_ASSERT(redraw_task()->context.GetCanvas());
//      frame_surface_->Damage(0, 0,
//                             width() + frame_surface_->margin().lr(),
//                             height() + frame_surface_->margin().tb());
//      frame_surface_->Commit();
//    } else {
//      RedrawTask *task = GetRedrawTask(view);
//      kRedrawTaskTail.PushFront(task);
//      task->context = main_surface_;
//      DBG_ASSERT(task->context.GetCanvas());
//      main_surface_->Damage(view->x() + main_surface_->margin().left,
//                            view->y() + main_surface_->margin().top,
//                            view->width(),
//                            view->height());
//      main_surface_->Commit();
//    }
//  } else {
//    DBG_ASSERT(view == this);
//    kRedrawTaskTail.PushFront(redraw_task().get());
//    surface_->Commit();
//  }
}

ViewSurface *EGLWindow::OnGetSurface(const AbstractView *view) const {
//  return main_surface_;
  return nullptr;
}

void EGLWindow::OnResize(int width, int height) {
  wayland::Region input_region;

  if (frame_surface_) {
    Rect input_rect(width, height);
    input_rect.left = frame_surface_->margin().left - AbstractWindowFrame::kResizingMargin.left;
    input_rect.top = frame_surface_->margin().top - AbstractWindowFrame::kResizingMargin.top;
    input_rect.Resize(width + AbstractWindowFrame::kResizingMargin.lr(),
                      height + AbstractWindowFrame::kResizingMargin.tb());
    ResizeWindowFrame(window_frame(), width, height);

    input_region.Setup(Display::wl_compositor());
    input_region.Add((int) input_rect.x(),
                     (int) input_rect.y(),
                     (int) input_rect.width(),
                     (int) input_rect.height());
    frame_surface_->SetInputRegion(input_region);

    // Reset buffer:
    width += frame_surface_->margin().lr();
    height += frame_surface_->margin().tb();

    int total_size = width * 4 * height;
    if (total_size > frame_pool_.size()) {
      DBG_PRINT_MSG("size_required: %d, pool size: %d, %s\n", total_size, frame_pool_.size(), "Re-generate shm pool");
      frame_pool_.Setup(total_size);
      main_pool_.Setup(total_size);
    }
    frame_buffer_.Setup(frame_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
    frame_surface_->Attach(&frame_buffer_);
    frame_surface_->GetCanvas()->Clear();
    main_buffer_.Setup(main_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
    main_surface_->Attach(&main_buffer_);
    main_surface_->GetCanvas()->Clear();
  } else {
    input_region.Setup(Display::wl_compositor());
    input_region.Add(0, 0, width, height);
    surface_->SetInputRegion(input_region);
  }

  Rect client_rect = GetClientGeometry();
  surface_->Resize((int) client_rect.width(), (int) client_rect.height());
  OnResizeEGL(this->width(), this->height());

  UpdateAll();
}

void EGLWindow::OnDraw(const Context *context) {
  if (window_frame()) {
    DrawWindowFrame(window_frame(), context);
  }

  if (!animating_) {
    animating_ = true;
    frame_callback_.Setup(surface_->wl_surface());
    OnInitializeEGL();
  }
}

void EGLWindow::OnInitializeEGL() {

}

void EGLWindow::OnResizeEGL(int width, int height) {

}

void EGLWindow::OnRenderEGL() {
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

bool EGLWindow::MakeCurrent() {
  return surface_->MakeCurrent();
}

void EGLWindow::SwapBuffers() {
  if (surface_->SwapBuffers())
    surface_->parent()->Commit();
}

void EGLWindow::OnFrame(uint32_t serial) {
  frame_callback_.Setup(surface_->wl_surface());
  OnRenderEGL();
}

void EGLWindow::OnRelease() {
//  fprintf(stderr, "on release\n");
}

}
