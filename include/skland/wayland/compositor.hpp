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

#ifndef SKLAND_WAYLAND_COMPOSITOR_HPP_
#define SKLAND_WAYLAND_COMPOSITOR_HPP_

#include "registry.hpp"

namespace skland {
namespace wayland {

class Surface;
class Region;

class Compositor {

  friend class Surface;
  friend class Region;

  Compositor(const Compositor &) = delete;
  Compositor &operator=(const Compositor &) = delete;

 public:

  Compositor()
      : wl_compositor_(nullptr) {
  }

  ~Compositor() {
    if (wl_compositor_) wl_compositor_destroy(wl_compositor_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    wl_compositor_ =
        static_cast<struct wl_compositor *>(registry.Bind(id, &wl_compositor_interface, version));
  }

  void Destroy() {
    if (wl_compositor_) {
      wl_compositor_destroy(wl_compositor_);
      wl_compositor_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_compositor_set_user_data(wl_compositor_, user_data);
  }

  void *GetUserData() const {
    return wl_compositor_get_user_data(wl_compositor_);
  }

  uint32_t GetVersion() const {
    return wl_compositor_get_version(wl_compositor_);
  }

  bool IsValid() const {
    return nullptr != wl_compositor_;
  }

  bool IsNull() const {
    return nullptr == wl_compositor_;
  }

  bool Equal(const void *object) const {
    return wl_compositor_ == object;
  }

 private:

  struct wl_compositor *wl_compositor_;
};

}
}

#endif // SKLAND_WAYLAND_COMPOSITOR_HPP_
