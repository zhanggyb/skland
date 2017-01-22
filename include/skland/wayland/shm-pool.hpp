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

#ifndef SKLAND_WAYLAND_SHM_POOL_HPP_
#define SKLAND_WAYLAND_SHM_POOL_HPP_

#include "shm.hpp"

namespace skland {
namespace wayland {

class Buffer;

class ShmPool {

  friend class Buffer;

 public:

  ShmPool()
      : wl_shm_pool_(nullptr) {}

  ~ShmPool() {
    if (wl_shm_pool_) wl_shm_pool_destroy(wl_shm_pool_);
  }

  void Setup(const Shm &shm, int32_t fd, int32_t size) {
    Destroy();

    wl_shm_pool_ = wl_shm_create_pool(shm.wl_shm_, fd, size);
  }

  void Destroy() {
    if (wl_shm_pool_) {
      wl_shm_pool_destroy(wl_shm_pool_);
      wl_shm_pool_ = nullptr;
    }
  }

  /**
 * @ingroup iface_wl_shm_pool
 *
 * This request will cause the server to remap the backing memory
 * for the pool from the file descriptor passed when the pool was
 * created, but using the new size.  This request can only be
 * used to make the pool bigger.
 */
  void Resize(int32_t size) const {
    wl_shm_pool_resize(wl_shm_pool_, size);
  }

  void SetUserData(void *user_data) {
    wl_shm_pool_set_user_data(wl_shm_pool_, user_data);
  }

  void *GetUserData() const {
    return wl_shm_pool_get_user_data(wl_shm_pool_);
  }

  uint32_t GetVersion() const {
    return wl_shm_pool_get_version(wl_shm_pool_);
  }

  bool IsValid() const {
    return nullptr != wl_shm_pool_;
  }

 private:

  struct wl_shm_pool *wl_shm_pool_;
};

}
}

#endif // SKLAND_WAYLAND_SHM_POOL_HPP_
