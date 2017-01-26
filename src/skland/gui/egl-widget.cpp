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
#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>
#include "internal/redraw-task.hpp"

#include <stdlib.h>
#include <assert.h>

namespace skland {

EGLWidget::EGLWidget()
    : EGLWidget(400, 300) {
}

EGLWidget::EGLWidget(int width, int height)
    : AbstractWidget(width, height),
      surface_(nullptr),
      resize_(false),
      animating_(false),
      root_(nullptr) {
  surface_ = new EGLSurface(this);

  // Set empty regions
  empty_opaque_region_.Setup(Display::wl_compositor());
  surface_->SetOpaqueRegion(empty_opaque_region_);
  empty_input_region_.Setup(Display::wl_compositor());
  surface_->SetInputRegion(empty_input_region_);

  frame_callback_.done().Set(this, &EGLWidget::OnFrame);
}

EGLWidget::~EGLWidget() {
  delete surface_;
}

void EGLWidget::OnUpdate(AbstractView *view) {
  DBG_ASSERT(view == this);

  if (!surface_->wl_sub_surface().IsValid()) {
    if (nullptr == parent_view()) return;

    AbstractSurface *main_surface = parent_view()->GetSurface();
    if (nullptr == main_surface) return;

    root_ = main_surface;

    main_surface->AddSubSurface(surface_);
    surface_->SetDesync();
    surface_->SetPosition(main_surface->margin().l + (int) geometry().x(),
                          main_surface->margin().t + (int) geometry().y());
    surface_->Resize((int) geometry().width(), (int) geometry().height());

    if (surface_->MakeCurrent()) {
      OnInitializeEGL();
      surface_->SwapBuffers();
    }

//    OnFrame(0);
  }

  AbstractWidget::OnUpdate(view);
}

AbstractSurface *EGLWidget::OnGetSurface(const AbstractView * /* view */) const {
  return surface_;
}

void EGLWidget::OnResize(int width, int height) {
  resize(width, height);
  resize_ = true;
  Update();
}

void EGLWidget::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void EGLWidget::OnMouseLeave(MouseEvent *event) {
  event->Accept();
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
    animating_ = true;
    OnFrame(0);
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
  glClearColor(0.36, 0.85, 0.27, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}

void EGLWidget::OnFrame(uint32_t /* serial */) {
  static int count = 0;
  count++;
  fprintf(stderr, "on frame: %d\n", count);
  if (surface_->MakeCurrent()) {
    if (resize_) {
      resize_ = false;
      surface_->Resize((int) geometry().width(), (int) geometry().height());
    }
    frame_callback_.Setup(root_->wl_surface());
    OnRenderEGL();
    surface_->SwapBuffers();
    root_->Commit();
  }
}

}
