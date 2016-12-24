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

#ifndef SKLAND_WAYLAND_REGION_HPP_
#define SKLAND_WAYLAND_REGION_HPP_

#include "compositor.hpp"

namespace skland {
namespace wayland {

class Region {

  friend class Surface;

  Region(const Region &) = delete;
  Region &operator=(const Region &)  = delete;

 public:

  Region()
      : wl_region_(nullptr) {}

  ~Region() {
    if (wl_region_) wl_region_destroy(wl_region_);
  }

  void Setup(const Compositor &compositor) {
    Destroy();

    wl_region_ = wl_compositor_create_region(compositor.wl_compositor_);
  }

  void Destroy() {
    if (wl_region_) {
      wl_region_destroy(wl_region_);
      wl_region_ = nullptr;
    }
  }

  void Add(int32_t x, int32_t y, int32_t width, int32_t height) const {
    wl_region_add(wl_region_, x, y, width, height);
  }

  void Subtract(int32_t x, int32_t y, int32_t width, int32_t height) const {
    wl_region_subtract(wl_region_, x, y, width, height);
  }

  void SetUserData(void *user_data) {
    wl_region_set_user_data(wl_region_, user_data);
  }

  void *GetUserData() const {
    return wl_region_get_user_data(wl_region_);
  }

  uint32_t GetVersion() const {
    return wl_region_get_version(wl_region_);
  }

  bool IsValid() const {
    return nullptr != wl_region_;
  }

  bool IsNull() const {
    return nullptr == wl_region_;
  }

 private:

  struct wl_region *wl_region_;

};

}
}

#endif // SKLAND_WAYLAND_REGION_HPP_
