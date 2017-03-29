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
#include "display-private.hpp"

namespace skland {

/**
 * @ingroup gui_intern
 * @brief An internal class to get wayland registry from a Display
 */
SKLAND_NO_EXPORT class Display::Registry {

  Registry(const Registry &) = delete;
  Registry &operator=(const Registry &) = delete;

 public:

  Registry()
      : display_(nullptr) {
    display_ = Display::kDisplay;
  }

  ~Registry() {}

  const wayland::Display &wl_display() const {
    return display_->p_->wl_display;
  }

  const wayland::Registry &wl_registry() const {
    return display_->p_->wl_registry;
  }

  const wayland::Compositor &wl_compositor() const {
    return display_->p_->wl_compositor;
  }

  const wayland::SubCompositor &wl_subcompositor() const {
    return display_->p_->wl_subcompositor;
  }

  const wayland::Shm &wl_shm() const {
    return display_->p_->wl_shm;
  }

  const wayland::XdgShell &xdg_shell() const {
    return display_->p_->xdg_shell;
  }

  const wayland::Shell &wl_shell() const {
    return display_->p_->wl_shell;
  }

  const wayland::DataDeviceManager &wl_data_device_manager() const {
    return display_->p_->wl_data_device_manager;
  }

  const egl::Display &egl_display() const {
    return display_->p_->egl_display;
  }

  struct xkb_context *xkb_context() const {
    return display_->p_->xkb_context;
  }

 private:

  const Display *display_;

};

}

#endif // SKLAND_GUI_INTERNAL_DISPLAY_PROXY_HPP_
