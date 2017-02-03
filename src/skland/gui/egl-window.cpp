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
#include <skland/gui/application.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include "internal/redraw-task.hpp"

#include <skland/core/numeric.hpp>

#include <GLES2/gl2.h>

namespace skland {

EGLWindow::EGLWindow(const char *title, AbstractWindowFrame *frame)
    : EGLWindow(400, 300, title, frame) {

}

EGLWindow::EGLWindow(int width, int height, const char *title, AbstractWindowFrame *frame)
    : AbstractWindow(width, height, title, frame),
      surface_(nullptr),
      shown_(false) {
  surface_ = new EGLSurface(this);

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(0, 0, this->width(), this->height());
  surface_->SetInputRegion(input_region_);

  SetShellSurface(surface_);

  SetTitle(title);

  frame_callback_.done().Set(this, &EGLWindow::OnFrame);
}

EGLWindow::~EGLWindow() {
}

void EGLWindow::OnShown() {
  shown_ = true;

  if (surface_->MakeCurrent()) {
    OnInitializeEGL();
    surface_->SwapBuffers();
  }

  Update();
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

void EGLWindow::OnUpdate(AbstractView *view) {
  DBG_ASSERT(view == this);
  if (!shown_) return;

  OnFrame(0);
}

AbstractSurface *EGLWindow::OnGetSurface(const AbstractView *view) const {
  return surface_;
}

void EGLWindow::OnResize(int width, int height) {
  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(0, 0, width, height);
  surface_->SetInputRegion(input_region_);

  surface_->Resize(this->width(), this->height());
//      surface_->Commit();
  OnResizeEGL(width, height);

  Update();
}

void EGLWindow::OnDraw(const Context * /*context*/) {

}

void EGLWindow::OnFrame(uint32_t serial) {
  static int count = 0;
  count++;
  fprintf(stderr, "on frame: %d\n", count);
  if (surface_->MakeCurrent()) {
    frame_callback_.Setup(surface_->wl_surface());
    OnRenderEGL();
    surface_->SwapBuffers();
  }
}

}
