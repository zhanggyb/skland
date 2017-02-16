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

#ifndef SKLAND_EGL_SURFACE_HPP
#define SKLAND_EGL_SURFACE_HPP

#include "view-surface-holder.hpp"
#include "../egl/surface.hpp"

namespace skland {

class EGLSurface : public Trackable {

  EGLSurface() = delete;
  EGLSurface(const EGLSurface &) = delete;
  EGLSurface &operator=(const EGLSurface &) = delete;

 public:

  EGLSurface(ViewSurface *view_surface);

  virtual ~EGLSurface();

  bool MakeCurrent();

  bool SwapBuffers();

  bool SwapBuffersWithDamage(int x, int y, int width, int height);

  bool SwapInterval(EGLint interval = 0);

  void Resize(int width, int height, int dx = 0, int dy = 0) {
    egl_surface_.Resize(width, height, dx, dy);
  }

  ViewSurface *view_surface() const { return view_surface_holder_.view_surface(); }

 private:

  void OnViewSurfaceDestroying(__SLOT__);

  ViewSurfaceHolder view_surface_holder_;

  egl::Surface egl_surface_;

};

}

#endif //SKLAND_EGL_SURFACE_HPP
