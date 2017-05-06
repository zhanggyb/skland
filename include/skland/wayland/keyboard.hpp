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

#ifndef SKLAND_WAYLAND_KEYBOARD_HPP_
#define SKLAND_WAYLAND_KEYBOARD_HPP_

#include "seat.hpp"

namespace skland {
namespace wayland {

class Keyboard {

  Keyboard(const Keyboard &) = delete;
  Keyboard &operator=(const Keyboard &) = delete;

 public:

  Keyboard()
      : wl_keyboard_(nullptr) {}

  ~Keyboard() {
    if (wl_keyboard_) wl_keyboard_destroy(wl_keyboard_);
  }

  void Setup(const Seat &seat) {
    Destroy();
    wl_keyboard_ = wl_seat_get_keyboard(seat.wl_seat_);
  }

  void Destroy() {
    if (wl_keyboard_) {
      wl_keyboard_destroy(wl_keyboard_);
      wl_keyboard_ = nullptr;
    }
  }

  void AddListener(const struct wl_keyboard_listener *listener, void *user_data) {
    wl_keyboard_add_listener(wl_keyboard_, listener, user_data);
  }

  void SetUserData(void *user_data) {
    wl_keyboard_set_user_data(wl_keyboard_, user_data);
  }

  void *GetUserData() const {
    return wl_keyboard_get_user_data(wl_keyboard_);
  }

  uint32_t GetVersion() const {
    return wl_keyboard_get_version(wl_keyboard_);
  }

  bool IsValid() const {
    return nullptr != wl_keyboard_;
  }

 private:

  struct wl_keyboard *wl_keyboard_;

};

}
}

#endif // SKLAND_WAYLAND_KEYBOARD_HPP_
