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

#ifndef SKLAND_WAYLAND_CLIENT_POINTER_HPP_
#define SKLAND_WAYLAND_CLIENT_POINTER_HPP_

#include "surface.hpp"
#include "seat.hpp"

namespace skland {
namespace wayland {
namespace client {

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
    wl_pointer_add_listener(wl_pointer_, &kListener, this);
  }

  void Destroy() {
    if (wl_pointer_) {
      wl_pointer_destroy(wl_pointer_);
      wl_pointer_ = nullptr;
    }
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
    return wl_pointer_ != nullptr;
  }

  bool IsNull() const {
    return wl_pointer_ == nullptr;
  }

  DelegateRef<void(uint32_t, struct wl_surface *,
                   wl_fixed_t, wl_fixed_t)> enter() {
    return enter_;
  }

  DelegateRef<void(uint32_t, struct wl_surface *)> leave() {
    return leave_;
  }

  DelegateRef<void(uint32_t, wl_fixed_t, wl_fixed_t)> motion() {
    return motion_;
  }

  DelegateRef<void(uint32_t, uint32_t, uint32_t, uint32_t)> button() {
    return button_;
  }

  DelegateRef<void(uint32_t, uint32_t, wl_fixed_t)> axis() {
    return axis_;
  }

  DelegateRef<void()> frame() {
    return frame_;
  }

  DelegateRef<void(uint32_t)> axis_source() {
    return axis_source_;
  }

  DelegateRef<void(uint32_t, uint32_t)> axis_stop() {
    return axis_stop_;
  }

  DelegateRef<void(uint32_t, int32_t)> axis_discrete() {
    return axis_discrete_;
  }

 private:

  static void OnEnter(void *data,
                      struct wl_pointer *wl_pointer,
                      uint32_t serial,
                      struct wl_surface *wl_surface,
                      wl_fixed_t surface_x,
                      wl_fixed_t surface_y);

  static void OnLeave(void *data,
                      struct wl_pointer *wl_pointer,
                      uint32_t serial,
                      struct wl_surface *wl_surface);

  static void OnMotion(void *data,
                       struct wl_pointer *wl_pointer,
                       uint32_t time,
                       wl_fixed_t surface_x,
                       wl_fixed_t surface_y);

  static void OnButton(void *data,
                       struct wl_pointer *wl_pointer,
                       uint32_t serial,
                       uint32_t time,
                       uint32_t button,
                       uint32_t state);

  static void OnAxis(void *data,
                     struct wl_pointer *wl_pointer,
                     uint32_t time,
                     uint32_t axis,
                     wl_fixed_t value);

  static void OnFrame(void *data,
                      struct wl_pointer *wl_pointer);

  static void OnAxisSource(void *data,
                           struct wl_pointer *wl_pointer,
                           uint32_t axis_source);

  static void OnAxisStop(void *data,
                         struct wl_pointer *wl_pointer,
                         uint32_t time,
                         uint32_t axis);

  static void OnAxisDiscrete(void *data,
                             struct wl_pointer *wl_pointer,
                             uint32_t axis,
                             int32_t discrete);

  static const struct wl_pointer_listener kListener;

  struct wl_pointer *wl_pointer_;

  Delegate<void(uint32_t,
                struct wl_surface *,
                wl_fixed_t,
                wl_fixed_t)> enter_;

  Delegate<void(uint32_t,
                struct wl_surface *)> leave_;

  Delegate<void(uint32_t,
                wl_fixed_t,
                wl_fixed_t)> motion_;

  Delegate<void(uint32_t,
                uint32_t,
                uint32_t,
                uint32_t)> button_;

  Delegate<void(uint32_t,
                uint32_t,
                wl_fixed_t)> axis_;

  Delegate<void()> frame_;

  Delegate<void(uint32_t)> axis_source_;

  Delegate<void(uint32_t, uint32_t)> axis_stop_;

  Delegate<void(uint32_t, int32_t)> axis_discrete_;
};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_POINTER_HPP_
