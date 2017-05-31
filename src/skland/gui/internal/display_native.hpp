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

#ifndef SKLAND_GUI_INTERNAL_DISPLAY_REGISTRY_HPP_
#define SKLAND_GUI_INTERNAL_DISPLAY_REGISTRY_HPP_

#include <skland/gui/display.hpp>
#include "display_private.hpp"

namespace skland {

/**
 * @ingroup gui_intern
 * @brief An internal class to get wayland registry from a Display
 */
SKLAND_NO_EXPORT class Display::Native {

  Native(const Native &) = delete;
  Native &operator=(const Native &) = delete;

 public:

  Native()
      : display_(nullptr) {
    display_ = Display::kDisplay;
  }

  ~Native() {}

  struct wl_display *wl_display() const {
    return display_->p_->wl_display;
  }

  struct wl_registry *wl_registry() const {
    return display_->p_->wl_registry;
  }

  struct wl_compositor *wl_compositor() const {
    return display_->p_->wl_compositor;
  }

  struct wl_subcompositor *wl_subcompositor() const {
    return display_->p_->wl_subcompositor;
  }

  struct wl_shm *wl_shm() const {
    return display_->p_->wl_shm;
  }

  struct zxdg_shell_v6 *xdg_shell() const {
    return display_->p_->xdg_shell;
  }

  struct wl_shell *wl_shell() const {
    return display_->p_->wl_shell;
  }

  struct wl_data_device_manager *wl_data_device_manager() const {
    return display_->p_->wl_data_device_manager;
  }

  EGLDisplay egl_display() const {
    return display_->p_->egl_display;
  }

  EGLConfig egl_config() const {
    return display_->p_->egl_config;
  }

  EGLContext egl_context() const {
    return display_->p_->egl_context;
  }

  struct xkb_context *xkb_context() const {
    return display_->p_->xkb_context;
  }

  static PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC kSwapBuffersWithDamageAPI;

 private:

  const Display *display_;

};

}

#endif // SKLAND_GUI_INTERNAL_DISPLAY_PROXY_HPP_