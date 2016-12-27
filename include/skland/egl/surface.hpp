//
// Created by zhanggyb on 16-12-24.
//

#ifndef SKLAND_SURFACE_HPP
#define SKLAND_SURFACE_HPP

#include <EGL/egl.h>
#include <wayland-egl.h>

namespace skland {

namespace wayland {
class Surface;
}

namespace egl {

class Display;

class Surface {

  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

 public:

  Surface();

  ~Surface();

  void Setup(const egl::Display &egl_display,
             const wayland::Surface &wl_surface,
             int width, int height);

  void Destroy();

  void Resize(int width, int height, int dx, int dy) {
    wl_egl_window_resize(wl_egl_window_, width, height, dx, dy);
  }

  bool IsValid() const {
    return nullptr != egl_surface_;
  }

  bool IsNull() const {
    return nullptr == egl_surface_;
  }

 private:

  EGLSurface egl_surface_;
  struct wl_egl_window *wl_egl_window_;
};

}
}

#endif //SKLAND_SURFACE_HPP
