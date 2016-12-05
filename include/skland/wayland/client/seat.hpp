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

#ifndef SKLAND_WAYLAND_CLIENT_SEAT_HPP_
#define SKLAND_WAYLAND_CLIENT_SEAT_HPP_

#include "registry.hpp"

namespace skland {
namespace wayland {
namespace client {

class Pointer;
class Keyboard;
class Touch;
class XdgToplevel;

class Seat {

  friend class Pointer;
  friend class Keyboard;
  friend class Touch;
  friend class XdgToplevel;

  Seat(const Seat &) = delete;
  Seat &operator=(const Seat &) = delete;

 public:

  Seat()
      : wl_seat_(nullptr) {}

  ~Seat() {
    if (wl_seat_) wl_seat_destroy(wl_seat_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    wl_seat_ = static_cast<struct wl_seat *>(registry.Bind(id, &wl_seat_interface, version));
    wl_seat_add_listener(wl_seat_, &kListener, this);
  }

  void Destroy() {
    if (wl_seat_) {
      wl_seat_destroy(wl_seat_);
      wl_seat_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_seat_set_user_data(wl_seat_, user_data);
  }

  void *GetUserData() const {
    return wl_seat_get_user_data(wl_seat_);
  }

  uint32_t GetVersion() const {
    return wl_seat_get_version(wl_seat_);
  }

  bool IsValid() const {
    return wl_seat_ != nullptr;
  }

  bool IsNull() const {
    return wl_seat_ == nullptr;
  }

  bool Equal(const void *object) const {
    return wl_seat_ == object;
  }

  DelegateRef<void(uint32_t)> capabilities() {
    return capabilities_;
  }

  DelegateRef<void(const char *)> name() {
    return name_;
  }

 private:

  static void OnCapabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities);

  static void OnName(void *data, struct wl_seat *wl_seat, const char *name);

  static const struct wl_seat_listener kListener;

  struct wl_seat *wl_seat_;

  Delegate<void(uint32_t)> capabilities_;
  Delegate<void(const char *)> name_;
};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_SEAT_HPP_
