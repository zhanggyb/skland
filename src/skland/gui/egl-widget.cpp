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

#include <skland/gui/egl-widget.hpp>

#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include "internal/abstract-shell-view-redraw-task.hpp"
#include "internal/abstract-view-iterators.hpp"

#include <GLES2/gl2.h>

#include <stdlib.h>
#include <assert.h>

namespace skland {

EGLWidget::EGLWidget()
    : EGLWidget(400, 300) {
}

EGLWidget::EGLWidget(int width, int height)
    : AbstractView(width, height),
      sub_surface_(nullptr),
      egl_surface_(nullptr),
      animating_(false) {
  frame_callback_.done().Set(this, &EGLWidget::OnFrame);
}

EGLWidget::~EGLWidget() {
  delete egl_surface_;
  delete sub_surface_;
}

void EGLWidget::OnUpdate(AbstractView *view) {
  DBG_ASSERT(view == this);

  if (nullptr == sub_surface_) {
    DBG_ASSERT(nullptr == egl_surface_);
    Iterator it(this);
    if (nullptr == it.parent()) return;

    it = it.parent();
    Surface *parent_surface = it.GetSurface();
    if (nullptr == parent_surface) return;

    sub_surface_ = Surface::Sub::Create(parent_surface, this);
    egl_surface_ = Surface::EGL::Get(sub_surface_);
    Surface::Sub::Get(sub_surface_)->SetWindowPosition(GetX(), GetY());
    egl_surface_->Resize(GetWidth(), GetHeight());
//    surface_->SetDesync();
  }

  AbstractView::OnUpdate(view);
}

Surface *EGLWidget::GetSurface(const AbstractView * /* view */) const {
  return sub_surface_;
}

void EGLWidget::OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  Update();
}

void EGLWidget::OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  egl_surface_->Resize(GetWidth(), GetHeight());
  OnResizeEGL();
}

void EGLWidget::OnLayout(int, int, int, int, int) {

}

void EGLWidget::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnMouseLeave() {

}

void EGLWidget::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnMouseButton(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void EGLWidget::OnDraw(const Context *context) {
  if (!animating_) {
    if (egl_surface_->MakeCurrent()) {
      animating_ = true;
      OnInitializeEGL();
      egl_surface_->surface()->SetupCallback(frame_callback_);
      egl_surface_->SwapBuffers();
      egl_surface_->surface()->Commit();
    }
  }
}

void EGLWidget::OnInitializeEGL() {
  glClearColor(0.1, 0.1, .85, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void EGLWidget::OnResizeEGL() {

}

void EGLWidget::OnRenderEGL() {
  glClearColor(0.36, 0.85, 0.27, 0.9);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void EGLWidget::OnFrame(uint32_t /* serial */) {
  static int count = 0;
  count++;
  fprintf(stderr, "on frame: %d\n", count);
  if (egl_surface_->MakeCurrent()) {
    OnRenderEGL();
    egl_surface_->surface()->SetupCallback(frame_callback_);
    egl_surface_->SwapBuffers();
    egl_surface_->surface()->Commit();
  }
}

}
