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

#ifndef SKLAND_WAYLAND_CLIENT_KEYBOARD_HPP_
#define SKLAND_WAYLAND_CLIENT_KEYBOARD_HPP_

#include "seat.hpp"
#include <skland/core/delegate.hpp>

namespace skland {
namespace wayland {
namespace client {

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
    wl_keyboard_add_listener(wl_keyboard_, &kListener, this);
  }

  void Destroy() {
    if (wl_keyboard_) {
      wl_keyboard_destroy(wl_keyboard_);
      wl_keyboard_ = nullptr;
    }
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
    return wl_keyboard_ != nullptr;
  }

  bool IsNull() const {
    return wl_keyboard_ == nullptr;
  }

  DelegateRef<void(uint32_t, int32_t, uint32_t)> keymap() {
    return keymap_;
  }

  DelegateRef<void(uint32_t, struct wl_surface *, struct wl_array *)> enter() {
    return enter_;
  }

  DelegateRef<void(uint32_t, struct wl_surface *)> leave() {
    return leave_;
  }

  DelegateRef<void(uint32_t, uint32_t, uint32_t, uint32_t)> key() {
    return key_;
  }

  DelegateRef<void(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)> modifiers() {
    return modifiers_;
  }

  DelegateRef<void(int32_t, int32_t)> repeat_info() {
    return repeat_info_;
  }

 private:

  static void OnKeymap(void *data,
                       struct wl_keyboard *wl_keyboard,
                       uint32_t format,
                       int32_t fd,
                       uint32_t size);

  static void OnEnter(void *data,
                      struct wl_keyboard *wl_keyboard,
                      uint32_t serial,
                      struct wl_surface *wl_surface,
                      struct wl_array *keys);

  static void OnLeave(void *data,
                      struct wl_keyboard *wl_keyboard,
                      uint32_t serial,
                      struct wl_surface *wl_surface);

  static void OnKey(void *data,
                    struct wl_keyboard *wl_keyboard,
                    uint32_t serial,
                    uint32_t time,
                    uint32_t key,
                    uint32_t state);

  static void OnModifiers(void *data,
                          struct wl_keyboard *wl_keyboard,
                          uint32_t serial,
                          uint32_t mods_depressed,
                          uint32_t mods_latched,
                          uint32_t mods_locked,
                          uint32_t group);

  static void OnRepeatInfo(void *data,
                           struct wl_keyboard *wl_keyboard,
                           int32_t rate,
                           int32_t delay);

  static const struct wl_keyboard_listener kListener;

  struct wl_keyboard *wl_keyboard_;

  Delegate<void(uint32_t, int32_t, uint32_t)> keymap_;

  Delegate<void(uint32_t, struct wl_surface *, struct wl_array *)> enter_;

  Delegate<void(uint32_t, struct wl_surface *)> leave_;

  Delegate<void(uint32_t, uint32_t, uint32_t, uint32_t)> key_;

  Delegate<void(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t)> modifiers_;

  Delegate<void(int32_t, int32_t)> repeat_info_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_KEYBOARD_HPP_
