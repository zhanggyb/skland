//
// Created by zhanggyb on 16-12-24.
//

#include <skland/egl/surface.hpp>
#include <skland/core/defines.hpp>
#include <skland/wayland/surface.hpp>
#include <skland/egl/display.hpp>

namespace skland {
namespace egl {

Surface::Surface()
    : egl_surface_(nullptr),
      wl_egl_window_(nullptr) {

}

Surface::~Surface() {
  if (egl_surface_) {
    DBG_ASSERT(wl_egl_window_);
    wl_egl_window_destroy(wl_egl_window_);
  }
}

void Surface::Setup(const egl::Display &egl_display,
                    const wayland::Surface &wl_surface,
                    int width, int height) {
  Destroy();

  wl_egl_window_ =
      wl_egl_window_create(wl_surface.wl_surface_, width, height);
  egl_surface_ =
      eglCreatePlatformWindowSurface(egl_display.egl_display_,
                                     egl_display.egl_config_,
                                     wl_egl_window_,
                                     NULL);
}

void Surface::Destroy() {
  if (egl_surface_) {
    DBG_ASSERT(wl_egl_window_);
    wl_egl_window_destroy(wl_egl_window_);
    wl_egl_window_ = nullptr;
    egl_surface_ = nullptr;
  }
}

}
}