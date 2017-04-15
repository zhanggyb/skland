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

#include <skland/egl/surface.hpp>
#include <skland/core/assert.hpp>
#include <skland/wayland/surface.hpp>
#include <skland/egl/display.hpp>

#include <stdlib.h>

namespace skland {
namespace egl {

Surface::Surface()
    : egl_surface_(nullptr),
      wl_egl_window_(nullptr) {

}

Surface::~Surface() {
  if (egl_surface_) {
    _ASSERT(wl_egl_window_);
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
    _ASSERT(wl_egl_window_);
    wl_egl_window_destroy(wl_egl_window_);
    wl_egl_window_ = nullptr;
    egl_surface_ = nullptr;
  }
}

}
}
