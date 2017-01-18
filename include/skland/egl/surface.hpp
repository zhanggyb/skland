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

#ifndef SKLAND_EGL_SURFACE_HPP_
#define SKLAND_EGL_SURFACE_HPP_

#include <EGL/egl.h>
#include <wayland-egl.h>

namespace skland {

// Forward declaration:
namespace wayland {
class Surface;
}

namespace egl {

class Display;

/**
 * @ingroup egl
 * @brief EGL Surface
 */
class Surface {

  friend class Display;

  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

 public:

  Surface();

  ~Surface();

  void Setup(const egl::Display &egl_display,
             const wayland::Surface &wl_surface,
             int width, int height);

  void Destroy();

  void Resize(int width, int height, int dx = 0, int dy = 0) {
    wl_egl_window_resize(wl_egl_window_, width, height, dx, dy);
  }

  bool IsValid() const {
    return nullptr != egl_surface_;
  }

  bool IsNull() const {
    return nullptr == egl_surface_;
  }

 private:

  ::EGLSurface egl_surface_;
  struct wl_egl_window *wl_egl_window_;
};

}
}

#endif // SKLAND_EGL_SURFACE_HPP_
