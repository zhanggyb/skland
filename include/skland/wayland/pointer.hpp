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

#ifndef SKLAND_WAYLAND_POINTER_HPP_
#define SKLAND_WAYLAND_POINTER_HPP_

#include "surface.hpp"
#include "seat.hpp"

namespace skland {
namespace wayland {

class Pointer {

  Pointer(const Pointer &) = delete;
  Pointer &operator=(const Pointer &) = delete;

 public:

  Pointer()
      : wl_pointer_(nullptr) {}

  ~Pointer() {
    if (wl_pointer_) wl_pointer_destroy(wl_pointer_);
  }

  void Setup(const Seat &seat) {
    Destroy();
    wl_pointer_ = wl_seat_get_pointer(seat.wl_seat_);
  }

  void Destroy() {
    if (wl_pointer_) {
      wl_pointer_destroy(wl_pointer_);
      wl_pointer_ = nullptr;
    }
  }

  void AddListener(const struct wl_pointer_listener *listener, void *user_data) {
    wl_pointer_add_listener(wl_pointer_, listener, user_data);
  }

  void SetCursor(uint32_t serial, const Surface &surface, int32_t hotspot_x, int32_t hotspot_y) const {
    wl_pointer_set_cursor(wl_pointer_, serial, surface.wl_surface_, hotspot_x, hotspot_y);
  }

  void SetUserData(void *user_data) const {
    wl_pointer_set_user_data(wl_pointer_, user_data);
  }

  void *GetUserData() const {
    return wl_pointer_get_user_data(wl_pointer_);
  }

  uint32_t GetVersion() const {
    return wl_pointer_get_version(wl_pointer_);
  }

  bool IsValid() const {
    return nullptr != wl_pointer_;
  }

 private:

  struct wl_pointer *wl_pointer_;

};

}
}

#endif // SKLAND_WAYLAND_POINTER_HPP_
