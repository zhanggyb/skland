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
namespace gui {

/**
 * @ingroup gui_intern
 * @brief An internal nested class to get native platform objects in Display
 */
SKLAND_NO_EXPORT class Display::Native {

  Native(const Native &) = delete;
  Native &operator=(const Native &) = delete;

 public:

  /**
   * @brief Default constructor, always use the global Display object
   */
  Native()
      : display_(nullptr) {
    display_ = Display::kDisplay;
  }

  /**
   * @brief Destructor
   */
  ~Native() {}

  /**
   * @brief Get the native wayland display object
   * @return
   */
  struct wl_display *wl_display() const {
    return display_->p_->wl_display;
  }

  /**
   * @brief Get the native wayland registry object
   * @return
   */
  struct wl_registry *wl_registry() const {
    return display_->p_->wl_registry;
  }

  /**
   * @brief Get the native wayland compositor object
   * @return
   */
  struct wl_compositor *wl_compositor() const {
    return display_->p_->wl_compositor;
  }

  /**
   * @brief Get the native wayland subcompositor object
   * @return
   */
  struct wl_subcompositor *wl_subcompositor() const {
    return display_->p_->wl_subcompositor;
  }

  /**
   * @brief Get the native wayland shm object
   * @return
   */
  struct wl_shm *wl_shm() const {
    return display_->p_->wl_shm;
  }

  /**
   * @brief Get the native xdg shell (in unstable protocol) object
   * @return
   */
  struct zxdg_shell_v6 *xdg_shell() const {
    return display_->p_->xdg_shell;
  }

  /**
   * @brief Get the native wayland shell object
   * @return
   */
  struct wl_shell *wl_shell() const {
    return display_->p_->wl_shell;
  }

  /**
   * @brief Get the native wayland data device manager
   * @return
   */
  struct wl_data_device_manager *wl_data_device_manager() const {
    return display_->p_->wl_data_device_manager;
  }

  /**
   * @brief Get the native EGL display
   * @return
   */
  EGLDisplay egl_display() const {
    return display_->p_->egl_display;
  }

  /**
   * @brief Get the native EGL config
   * @return
   */
  EGLConfig egl_config() const {
    return display_->p_->egl_config;
  }

  /**
   * @brief Get the native EGL context
   * @return
   */
  EGLContext egl_context() const {
    return display_->p_->egl_context;
  }

  /**
   * @brief Get the native C++ Vulkan instance
   * @return
   */
  const vk::Instance vk_instance() const {
    return display_->p_->vk_instance;
  }

  /**
   * @brief Get the native xkb context
   * @return The xkb_context object for keymap
   */
  struct xkb_context *xkb_context() const {
    return display_->p_->xkb_context;
  }

  static PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC kSwapBuffersWithDamageAPI;

 private:

  const Display *display_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_INTERNAL_DISPLAY_PROXY_HPP_
