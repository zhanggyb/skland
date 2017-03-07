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
#include <skland/gui/abstract-view.hpp>
#include <skland/gui/surface.hpp>

#include "internal/display-registry.hpp"

namespace skland {

EGLSurface *EGLSurface::Get(Surface *surface) {
  if (nullptr == surface->egl_surface_role_)
    surface->egl_surface_role_ = new EGLSurface(surface);

  return surface->egl_surface_role_;
}

EGLSurface::EGLSurface(Surface *surface)
    : surface_(surface) {
  egl_surface_.Setup(Display::Registry().egl_display(),
                     surface_->wl_surface_, 400, 400);
//                     surface->event_handler()->width(),
//                     surface->event_handler()->height());
}

EGLSurface::~EGLSurface() {
  egl_surface_.Destroy();
  surface_->egl_surface_role_ = nullptr;
}

bool EGLSurface::MakeCurrent() {
  return Display::Registry().egl_display().MakeCurrent(egl_surface_, egl_surface_);
}

bool EGLSurface::SwapBuffers() {
  return Display::Registry().egl_display().SwapBuffers(egl_surface_);
}

bool EGLSurface::SwapBuffersWithDamage(int x, int y, int width, int height) {
  return Display::Registry().egl_display().SwapBuffersWithDamage(egl_surface_, x, y, width, height);
}

bool EGLSurface::SwapInterval(EGLint interval) {
  return Display::Registry().egl_display().SwapInterval(interval);
}

} // namespace skland
