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

#ifndef SKLAND_WAYLAND_CALLBACK_HPP_
#define SKLAND_WAYLAND_CALLBACK_HPP_

#include "display.hpp"
#include "surface.hpp"

namespace skland {
namespace wayland {

/**
 * @ingroup wayland
 * @brief A callback to handle the 'done' event to get notified when the related
 * request is done.
 */
class Callback {

  Callback(const Callback &) = delete;
  Callback &operator=(const Callback &) = delete;

 public:

  Callback()
      : wl_callback_(nullptr) {}

  /**
   * @brief Create a callback to a display sync event
   */
  Callback(const Display &display)
      : wl_callback_(nullptr) {
    wl_callback_ = wl_display_sync(display.wl_display_);
  }

  /**
   * @brief Create a callback to a surface frame event
   */
  Callback(const Surface &surface)
      : wl_callback_(nullptr) {
    wl_callback_ = wl_surface_frame(surface.wl_surface_);
  }

  /**
   * @brief Destructor
   *
   * Destroy the native wayland callback object
   */
  ~Callback() {
    if (wl_callback_) wl_callback_destroy(wl_callback_);
  }

  void Setup(const Display &display) {
    Destroy();
    wl_callback_ = wl_display_sync(display.wl_display_);
  }

  void Setup(const Surface &surface) {
    Destroy();
    wl_callback_ = wl_surface_frame(surface.wl_surface_);
  }

  void Destroy() {
    if (wl_callback_) {
      wl_callback_destroy(wl_callback_);
      wl_callback_ = nullptr;
    }
  }

  void AddListener(const struct wl_callback_listener *listener, void *user_data) {
    wl_callback_add_listener(wl_callback_, listener, user_data);
  }

  bool IsValid() const {
    return nullptr != wl_callback_;
  }

 private:

  struct wl_callback *wl_callback_;

};

}
}

#endif // SKLAND_WAYLAND_CALLBACK_HPP_
