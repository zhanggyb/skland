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

#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>
#include <skland/gui/abstract-view.hpp>

namespace skland {

EGLSurface::EGLSurface(AbstractView *view)
    : AbstractSurface(view) {
  egl_surface_.Setup(Display::egl_display(), wl_surface(), view->width(), view->height());
}

EGLSurface::~EGLSurface() {

}

std::shared_ptr<Canvas> EGLSurface::GetCanvas() const {
  // TODO: render to canvas
  return nullptr;
}

bool EGLSurface::MakeCurrent() {
  return Display::egl_display().MakeCurrent(egl_surface_, egl_surface_);
}

bool EGLSurface::SwapBuffers() {
  return Display::egl_display().SwapBuffers(egl_surface_);
}

bool EGLSurface::SwapBuffersWithDamage(int x, int y, int width, int height) {
  return Display::egl_display().SwapBuffersWithDamage(egl_surface_, x, y, width, height);
}

bool EGLSurface::SwapInterval(EGLint interval) {
  return Display::egl_display().SwapInterval(interval);
}

// --------------------------------

EGLSurfaceExt::EGLSurfaceExt(ViewSurface *view_surface)
    : Trackable(), view_surface_holder_(view_surface) {
  view_surface_holder_.view_surface_destroying().Connect(this, &EGLSurfaceExt::OnViewSurfaceDestroying);
  egl_surface_.Setup(Display::egl_display(),
                     view_surface_holder_.wl_surface(),
                     view_surface->view()->width(),
                     view_surface->view()->height());
}

EGLSurfaceExt::~EGLSurfaceExt() {
  UnbindAll();
  egl_surface_.Destroy();
}

bool EGLSurfaceExt::MakeCurrent() {
  return Display::egl_display().MakeCurrent(egl_surface_, egl_surface_);
}

bool EGLSurfaceExt::SwapBuffers() {
  return Display::egl_display().SwapBuffers(egl_surface_);
}

bool EGLSurfaceExt::SwapBuffersWithDamage(int x, int y, int width, int height) {
  return Display::egl_display().SwapBuffersWithDamage(egl_surface_, x, y, width, height);
}

bool EGLSurfaceExt::SwapInterval(EGLint interval) {
  return Display::egl_display().SwapInterval(interval);
}

void EGLSurfaceExt::OnViewSurfaceDestroying(SLOT slot) {
  egl_surface_.Destroy();
}

} // namespace skland
