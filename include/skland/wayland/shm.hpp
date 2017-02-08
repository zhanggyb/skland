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

#ifndef SKLAND_WAYLAND_SHM_HPP_
#define SKLAND_WAYLAND_SHM_HPP_

#include "registry.hpp"

namespace skland {
namespace wayland {

class ShmPool;
class CursorTheme;

class Shm {

  friend class ShmPool;
  friend class CursorTheme;

  Shm(const Shm &) = delete;
  Shm &operator=(const Shm &) = delete;

 public:

  Shm()
      : wl_shm_(nullptr) {
  }

  ~Shm() {
    if (wl_shm_) wl_shm_destroy(wl_shm_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    wl_shm_ = static_cast<struct wl_shm *>(registry.Bind(id, &wl_shm_interface, version));
    wl_shm_add_listener(wl_shm_, &kListener, this);
  }

  void Destroy() {
    if (wl_shm_) {
      wl_shm_destroy(wl_shm_);
      wl_shm_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_shm_set_user_data(wl_shm_, user_data);
  }

  void *GetUserData() const {
    return wl_shm_get_user_data(wl_shm_);
  }

  uint32_t GetVersion() const {
    return wl_shm_get_version(wl_shm_);
  }

  bool IsValid() const {
    return nullptr != wl_shm_;
  }

  bool Equal(const void *object) const {
    return wl_shm_ == object;
  }

  DelegateRef<void(uint32_t)> format() {
    return format_;
  }

 private:

  static void OnFormat(void *data, struct wl_shm *shm, uint32_t format);

  static const struct wl_shm_listener kListener;

  struct wl_shm *wl_shm_;

  Delegate<void(uint32_t)> format_;
};

}
}

#endif // SKLAND_WAYLAND_SHM_HPP_
