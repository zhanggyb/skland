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

#ifndef SKLAND_WAYLAND_CLIENT_REGISTRY_HPP_
#define SKLAND_WAYLAND_CLIENT_REGISTRY_HPP_

#include "display.hpp"

namespace skland {
namespace wayland {
namespace client {

class Registry {

  Registry(const Registry &) = delete;
  Registry &operator=(const Registry &) = delete;

 public:

  Registry()
      : wl_registry_(nullptr) {
  }

  ~Registry() {
    if (wl_registry_) wl_registry_destroy(wl_registry_);
  }

  void Setup(const Display &display) {
    Destroy();

    wl_registry_ = wl_display_get_registry(display.wl_display_);
    wl_registry_add_listener(wl_registry_, &kListener, this);
  }

  void Destroy() {
    if (wl_registry_) {
      wl_registry_destroy(wl_registry_);
      wl_registry_ = nullptr;
    }
  }

  void *Bind(uint32_t name, const struct wl_interface *interface, uint32_t version) const {
    return wl_registry_bind(wl_registry_, name, interface, version);
  }

  void SetUserData(void *user_data) {
    wl_registry_set_user_data(wl_registry_, user_data);
  }

  void *GetUserData() const {
    return wl_registry_get_user_data(wl_registry_);
  }

  uint32_t GetVersion() const {
    return wl_registry_get_version(wl_registry_);
  }

  bool IsValid() const {
    return wl_registry_ != nullptr;
  }

  bool IsNull() const {
    return wl_registry_ == nullptr;
  }

  bool Equal(const void *object) const {
    return wl_registry_ == object;
  }

  DelegateRef<void(uint32_t, const char *, uint32_t)> global() {
    return global_;
  }

  DelegateRef<void(uint32_t)> global_remove() {
    return global_remove_;
  }

 private:

  static void OnGlobal(void *data,
                       struct wl_registry *registry,
                       uint32_t id,
                       const char *interface,
                       uint32_t version);

  static void OnGlobalRemove(void *data,
                             struct wl_registry *registry,
                             uint32_t name);

  static const struct wl_registry_listener kListener;

  struct wl_registry *wl_registry_;

  Delegate<void(uint32_t, const char *, uint32_t)> global_;
  Delegate<void(uint32_t)> global_remove_;
};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_REGISTRY_HPP_
