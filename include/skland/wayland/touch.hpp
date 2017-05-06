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

#ifndef SKLAND_WAYLAND_TOUCH_HPP_
#define SKLAND_WAYLAND_TOUCH_HPP_

#include "seat.hpp"

namespace skland {
namespace wayland {

class Touch {

  Touch(const Touch &) = delete;
  Touch &operator=(const Touch &) = delete;

 public:

  Touch()
      : wl_touch_(nullptr) {
  }

  ~Touch() {
    if (wl_touch_) wl_touch_destroy(wl_touch_);
  }

  void Setup(const Seat &seat) {
    Destroy();
    wl_touch_ = wl_seat_get_touch(seat.wl_seat_);
  }

  void Destroy() {
    if (wl_touch_) {
      wl_touch_destroy(wl_touch_);
      wl_touch_ = nullptr;
    }
  }

  void AddListener(const struct wl_touch_listener *listener, void *user_data) {
    wl_touch_add_listener(wl_touch_, listener, user_data);
  }

  void SetUserData(void *user_data) {
    wl_touch_set_user_data(wl_touch_, user_data);
  }

  void *GetUserData() const {
    return wl_touch_get_user_data(wl_touch_);
  }

  uint32_t GetVersion() const {
    return wl_touch_get_version(wl_touch_);
  }

  bool IsValid() const {
    return nullptr != wl_touch_;
  }

  DelegateRef<void(uint32_t, uint32_t, struct wl_surface *,
                   int32_t, wl_fixed_t, wl_fixed_t)> down() {
    return down_;
  }

  DelegateRef<void(uint32_t, uint32_t, int32_t)> up() { return up_; }

  DelegateRef<void(uint32_t, int32_t, wl_fixed_t, wl_fixed_t)> motion() { return motion_; }

  DelegateRef<void()> frame() { return frame_; }

  DelegateRef<void()> cancel() { return cancel_; }

 private:

  static void OnDown(void *data,
                     struct wl_touch *wl_touch,
                     uint32_t serial,
                     uint32_t time,
                     struct wl_surface *surface,
                     int32_t id,
                     wl_fixed_t x,
                     wl_fixed_t y);

  static void OnUp(void *data,
                   struct wl_touch *wl_touch,
                   uint32_t serial,
                   uint32_t time,
                   int32_t id);

  static void OnMotion(void *data,
                       struct wl_touch *wl_touch,
                       uint32_t time,
                       int32_t id,
                       wl_fixed_t x,
                       wl_fixed_t y);

  static void OnFrame(void *data,
                      struct wl_touch *wl_touch);

  static void OnCancel(void *data,
                       struct wl_touch *wl_touch);

  static const struct wl_touch_listener kListener;

  struct wl_touch *wl_touch_;

  Delegate<void(uint32_t,
                uint32_t,
                struct wl_surface *,
                int32_t,
                wl_fixed_t,
                wl_fixed_t)> down_;

  Delegate<void(uint32_t,
                uint32_t,
                int32_t)> up_;

  Delegate<void(uint32_t,
                int32_t,
                wl_fixed_t,
                wl_fixed_t)> motion_;

  Delegate<void()> frame_;

  Delegate<void()> cancel_;
};

}
}

#endif // SKLAND_WAYLAND_TOUCH_HPP_
