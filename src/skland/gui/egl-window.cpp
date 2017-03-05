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

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-shell-frame.hpp>

#include <skland/gui/surface.hpp>
#include <skland/gui/shell-surface.hpp>
#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/sub-surface.hpp>
#include <skland/gui/egl-surface.hpp>
#include <skland/gui/abstract-view.hpp>

#include <skland/graphic/canvas.hpp>

#include "internal/display-registry.hpp"
#include "internal/abstract-event-handler-redraw-task.hpp"
#include "internal/abstract-event-handler-redraw-task-iterator.hpp"

#include <GLES2/gl2.h>

namespace skland {

EGLWindow::EGLWindow(const char *title, AbstractShellFrame *frame)
    : EGLWindow(400, 300, title, frame) {

}

EGLWindow::EGLWindow(int width, int height, const char *title, AbstractShellFrame *frame)
    : AbstractShellView(width, height, title, nullptr, frame),
      main_surface_(nullptr),
      sub_surface_(nullptr),
      egl_surface_(nullptr),
      animating_(false) {
  Surface *parent = shell_surface();

  main_surface_ = SubSurface::Create(parent, this, parent->margin());
  DBG_ASSERT(main_surface_->parent() == parent);
  DBG_ASSERT(main_surface_->below() == parent);

  sub_surface_ = SubSurface::Create(main_surface_, this);
  DBG_ASSERT(sub_surface_->parent() == main_surface_);
  DBG_ASSERT(sub_surface_->below() == main_surface_);

  wayland::Region empty_region;
  empty_region.Setup(Display::Registry().wl_compositor());
  main_surface_->SetInputRegion(empty_region);
  sub_surface_->SetInputRegion(empty_region);

  Rect client_rect = GetClientGeometry();
  SubSurface::Get(sub_surface_)->SetWindowPosition((int) client_rect.l, (int) client_rect.t);
//    sub_surface_->SetDesync();  // FIXME: looks no difference between sync and desync modes

  egl_surface_ = EGLSurface::Get(sub_surface_);
  egl_surface_->Resize((int) client_rect.width(), (int) client_rect.height());

  frame_callback_.done().Set(this, &EGLWindow::OnFrame);
}

EGLWindow::~EGLWindow() {
  delete egl_surface_;
  delete sub_surface_;
  delete main_surface_;
}

void EGLWindow::OnShown() {
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

  // initialize EGL
//  UpdateAll();
}

void EGLWindow::OnUpdate(AbstractView *view) {
  if (!IsShown()) return;

  Surface *surface = nullptr;

  if (nullptr == view) {
    surface = this->shell_surface();
    RedrawTaskIterator redraw_task_helper(this);
    redraw_task_helper.MoveToTail();
    redraw_task_helper.SetContext(Context(surface, frame_canvas_));
    DBG_ASSERT(frame_canvas_);
    Damage(this, 0, 0,
           size().width + surface->margin().lr(),
           size().height + surface->margin().tb());
    surface->Commit();
  } else {
    std::shared_ptr<Canvas> canvas;
    surface = main_surface_;
    canvas = main_canvas_;

    RedrawTaskIterator redraw_task_helper(view);
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

Surface *EGLWindow::GetSurface(const AbstractView *view) const {
  if (nullptr == view)
    return shell_surface();

  return nullptr != sub_surface_ ? sub_surface_ : shell_surface();
}

void EGLWindow::OnSizeChanged(int width, int height) {
  RectI input_rect(width, height);
  Surface *shell_surface = this->shell_surface();

  input_rect.left = shell_surface->margin().left - AbstractShellFrame::kResizingMargin.left;
  input_rect.top = shell_surface->margin().top - AbstractShellFrame::kResizingMargin.top;
  input_rect.Resize(width + AbstractShellFrame::kResizingMargin.lr(),
                    height + AbstractShellFrame::kResizingMargin.tb());

  wayland::Region input_region;
  input_region.Setup(Display::Registry().wl_compositor());
  input_region.Add(input_rect.x(), input_rect.y(),
                   input_rect.width(), input_rect.height());
  shell_surface->SetInputRegion(input_region);

  ResizeShellFrame(shell_frame(), width, height);

  // Reset buffer:
  width += shell_surface->margin().lr();
  height += shell_surface->margin().tb();

  int total_size = width * 4 * height;
  frame_pool_.Setup(total_size);
  main_pool_.Setup(total_size);

  frame_buffer_.Setup(frame_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  shell_surface->Attach(&frame_buffer_);
  frame_canvas_.reset(new Canvas((unsigned char *) frame_buffer_.pixel(),
                                 frame_buffer_.size().width,
                                 frame_buffer_.size().height));
  frame_canvas_->SetOrigin(shell_surface->margin().left, shell_surface->margin().top);
  frame_canvas_->Clear();

  main_buffer_.Setup(main_pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  main_surface_->Attach(&main_buffer_);
  main_canvas_.reset(new Canvas((unsigned char *) main_buffer_.pixel(),
                                main_buffer_.size().width,
                                main_buffer_.size().height));
  main_canvas_->SetOrigin(main_surface_->margin().left,
                          main_surface_->margin().top);
  main_canvas_->Clear();

  Rect client_rect = GetClientGeometry();
  egl_surface_->Resize((int) client_rect.width(), (int) client_rect.height());
  OnResizeEGL(size().width, size().height);

//  UpdateAll();
}

void EGLWindow::OnDraw(const Context *context) {
  if (shell_frame()) DrawShellFrame(shell_frame(), context);

  if (!animating_) {
    animating_ = true;
    egl_surface_->surface()->SetupCallback(frame_callback_);
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
  return egl_surface_->MakeCurrent();
}

void EGLWindow::SwapBuffers() {
  if (egl_surface_->SwapBuffers())
    egl_surface_->surface()->Commit();
}

void EGLWindow::OnFrame(uint32_t serial) {
  egl_surface_->surface()->SetupCallback(frame_callback_);
  OnRenderEGL();
}

void EGLWindow::OnRelease() {
//  fprintf(stderr, "on release\n");
}

}
