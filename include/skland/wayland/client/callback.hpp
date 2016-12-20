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

#ifndef SKLAND_WAYLAND_CLIENT_CALLBACK_HPP_
#define SKLAND_WAYLAND_CLIENT_CALLBACK_HPP_

#include "display.hpp"
#include "surface.hpp"

namespace skland {
namespace wayland {
namespace client {

class Callback {

  Callback(const Callback &) = delete;
  Callback &operator=(const Callback &) = delete;

 public:

  Callback()
      : wl_callback_(nullptr) {}

  Callback(const Display &display)
      : wl_callback_(nullptr) {
    wl_callback_ = wl_display_sync(display.wl_display_);
    wl_callback_add_listener(wl_callback_, &kListener, this);
  }

  Callback(const Surface &surface)
      : wl_callback_(nullptr) {
    wl_callback_ = wl_surface_frame(surface.wl_surface_);
    wl_callback_add_listener(wl_callback_, &kListener, this);
  }

  ~Callback() {
    if (wl_callback_) wl_callback_destroy(wl_callback_);
  }

  void Setup(const Display &display) {
    Destroy();

    wl_callback_ = wl_display_sync(display.wl_display_);
    wl_callback_add_listener(wl_callback_, &kListener, this);
  }

  void Setup(const Surface &surface) {
    Destroy();

    wl_callback_ = wl_surface_frame(surface.wl_surface_);
    wl_callback_add_listener(wl_callback_, &kListener, this);
  }

  void Destroy() {
    if (wl_callback_) {
      wl_callback_destroy(wl_callback_);
      wl_callback_ = nullptr;
    }
  }

  DelegateRef<void(uint32_t)> done() {
    return done_;
  }

  bool IsValid() const {
    return nullptr != wl_callback_;
  }

  bool IsNull() const {
    return nullptr == wl_callback_;
  }

 private:

  static void OnDone(void *data,
                     struct wl_callback *wl_callback,
                     uint32_t callback_data);

  static const struct wl_callback_listener kListener;

  struct wl_callback *wl_callback_;

  Delegate<void(uint32_t)> done_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_CALLBACK_HPP_
