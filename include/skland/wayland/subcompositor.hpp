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

#ifndef SKLAND_WAYLAND_SUBCOMPOSITOR_HPP_
#define SKLAND_WAYLAND_SUBCOMPOSITOR_HPP_

#include "registry.hpp"

namespace skland {
namespace wayland {

class SubSurface;

class SubCompositor {

  friend class SubSurface;

  SubCompositor(const SubCompositor &) = delete;
  SubCompositor &operator=(const SubCompositor &) = delete;

 public:

  SubCompositor()
      : wl_subcompositor_(nullptr) {
  }

  ~SubCompositor() {
    if (wl_subcompositor_) wl_subcompositor_destroy(wl_subcompositor_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    wl_subcompositor_ =
        static_cast<struct wl_subcompositor *>(registry.Bind(id, &wl_subcompositor_interface, version));
  }

  void Destroy() {
    if (wl_subcompositor_) {
      wl_subcompositor_destroy(wl_subcompositor_);
      wl_subcompositor_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_subcompositor_set_user_data(wl_subcompositor_, user_data);
  }

  void *GetUserData() const {
    return wl_subcompositor_get_user_data(wl_subcompositor_);
  }

  uint32_t GetVersion() const {
    return wl_subcompositor_get_version(wl_subcompositor_);
  }

  bool IsValid() const {
    return nullptr != wl_subcompositor_;
  }

  bool Equal(const void *object) const {
    return wl_subcompositor_ == object;
  }

 private:

  struct wl_subcompositor *wl_subcompositor_;
};

}
}

#endif // SKLAND_WAYLAND_SUBCOMPOSITOR_HPP_
