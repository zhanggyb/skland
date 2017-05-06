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

#ifndef SKLAND_WAYLAND_BUFFER_HPP_
#define SKLAND_WAYLAND_BUFFER_HPP_

#include "shm-pool.hpp"

namespace skland {
namespace wayland {

class Surface;

class Buffer {

  friend class Surface;

  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

 public:

  Buffer()
      : wl_buffer_(nullptr) {}

  ~Buffer() {
    if (wl_buffer_) wl_buffer_destroy(wl_buffer_);
  }

  void Setup(const ShmPool &shm_pool,
             int32_t offset,
             int32_t width,
             int32_t height,
             int32_t stride,
             uint32_t format) {
    Destroy();
    wl_buffer_ = wl_shm_pool_create_buffer(shm_pool.wl_shm_pool_, offset, width, height, stride, format);
  }

  void AddListener(const struct wl_buffer_listener *listener, void *user_data) {
    wl_buffer_add_listener(wl_buffer_, listener, user_data);
  }

  void Destroy() {
    if (wl_buffer_) {
      wl_buffer_destroy(wl_buffer_);
      wl_buffer_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_buffer_set_user_data(wl_buffer_, user_data);
  }

  void *GetUserData() const {
    return wl_buffer_get_user_data(wl_buffer_);
  }

  uint32_t GetVersion() const {
    return wl_buffer_get_version(wl_buffer_);
  }

  bool IsValid() const {
    return nullptr != wl_buffer_;
  }

  bool Equal(const void *object) const {
    return wl_buffer_ == object;
  }

 private:

  struct wl_buffer *wl_buffer_;

};

}
}

#endif // SKLAND_WAYLAND_BUFFER_HPP_
