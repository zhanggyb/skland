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

EGLWindow::EGLWindow()
    : EGLWindow(400, 300) {

}

EGLWindow::EGLWindow(int width, int height)
    : AbstractView(width, height),
      is_xdg_surface_configured_(false),
      surface_(nullptr) {
  surface_ = new EGLSurface(this);

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(0, 0, (int) geometry().width(), (int) geometry().height());
  surface_->SetInputRegion(input_region_);

  xdg_surface_.configure().Set(this, &EGLWindow::OnXdgSurfaceConfigure);
  xdg_surface_.Setup(Display::xdg_shell(), surface_->wl_surface());

  xdg_toplevel_.configure().Set(this, &EGLWindow::OnXdgToplevelConfigure);
  xdg_toplevel_.close().Set(this, &EGLWindow::OnXdgToplevelClose);
  xdg_toplevel_.Setup(xdg_surface_);

  xdg_toplevel_.SetTitle("Test EGL surface"); // TODO: support multi-language
  xdg_toplevel_.SetAppId("Test EGL surface");

  frame_callback_.done().Set(this, &EGLWindow::OnFrame);
}

EGLWindow::~EGLWindow() {
  delete surface_;
}

void EGLWindow::Show() {
  if (!is_xdg_surface_configured_) {
    surface_->Commit();
  }
}

void EGLWindow::Close(SLOT) {
  if (AbstractSurface::GetShellSurfaceCount() == 1) {
    Application::Exit();
  }

  delete this;
}

Size EGLWindow::GetMinimalSize() const {
  return Size(100, 100);
}

Size EGLWindow::GetPreferredSize() const {
  return Size(400, 300);
}

Size EGLWindow::GetMaximalSize() const {
  return Size(65536, 65536);
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
  if (!is_xdg_surface_configured_) return;

  OnFrame(0);
}

AbstractSurface *EGLWindow::OnGetSurface(const AbstractView *view) const {
  return surface_;
}

void EGLWindow::OnResize(int width, int height) {
  resize(width, height);
  Update();
}

void EGLWindow::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void EGLWindow::OnMouseLeave(MouseEvent *event) {
  event->Accept();
}

void EGLWindow::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void EGLWindow::OnMouseButton(MouseEvent *event) {
  if (event->button() == kMouseButtonLeft && event->state() == kMouseButtonPressed) {
    xdg_toplevel_.Move(event->wl_seat(), event->serial());
    event->Ignore();
    return;
  }

  event->Accept();
}

void EGLWindow::OnKeyboardKey(KeyEvent *event) {

}

void EGLWindow::OnDraw(const Context * /*context*/) {

}

void EGLWindow::OnXdgSurfaceConfigure(uint32_t serial) {
  xdg_surface_.AckConfigure(serial);

  if (!is_xdg_surface_configured_) {
    is_xdg_surface_configured_ = true;

    xdg_surface_.SetWindowGeometry(0, 0, (int) geometry().width(), (int) geometry().height());
//    surface_->Resize((int) geometry().width(), (int) geometry().height());

    if (surface_->MakeCurrent()) {
      OnInitializeEGL();
      surface_->SwapBuffers();
    }

    Update();
  }
}

void EGLWindow::OnXdgToplevelConfigure(int width, int height, int states) {
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

      input_region_.Setup(Display::wl_compositor());
      input_region_.Add(0, 0, width, height);
      surface_->SetInputRegion(input_region_);

      xdg_surface_.SetWindowGeometry(0, 0, width, height);

      resize(width, height);

      surface_->Resize((int) geometry().width(), (int) geometry().height());
//      surface_->Commit();
      OnResizeEGL(width, height);

      OnResize(width, height);
    }
  }

}

void EGLWindow::OnXdgToplevelClose() {
  Close();
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
