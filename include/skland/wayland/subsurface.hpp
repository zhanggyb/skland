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

#ifndef SKLAND_WAYLAND_SUBSURFACE_HPP_
#define SKLAND_WAYLAND_SUBSURFACE_HPP_

#include "surface.hpp"
#include "subcompositor.hpp"

namespace skland {
namespace wayland {

class SubSurface {

 public:

  SubSurface()
      : wl_subsurface_(nullptr) {}

  ~SubSurface() {
    if (wl_subsurface_) wl_subsurface_destroy(wl_subsurface_);
  }

  void Setup(const SubCompositor &subcompositor, const Surface &surface, const Surface &parent) {
    Destroy();

    wl_subsurface_ = wl_subcompositor_get_subsurface(subcompositor.wl_subcompositor_,
                                                     surface.wl_surface_, parent.wl_surface_);
  }

  void Destroy() {
    if (wl_subsurface_) {
      wl_subsurface_destroy(wl_subsurface_);
      wl_subsurface_ = nullptr;
    }
  }

  void SetPosition(int32_t x, int32_t y) {
    wl_subsurface_set_position(wl_subsurface_, x, y);
  }

  void PlaceAbove(const Surface &sibling) {
    wl_subsurface_place_above(wl_subsurface_, sibling.wl_surface_);
  }

  void PlaceBelow(const Surface &sibling) {
    wl_subsurface_place_below(wl_subsurface_, sibling.wl_surface_);
  }

  void SetSync() const {
    wl_subsurface_set_sync(wl_subsurface_);
  }

  void SetDesync() const {
    wl_subsurface_set_desync(wl_subsurface_);
  }

  void SetUserData(void *user_data) {
    wl_subsurface_set_user_data(wl_subsurface_, user_data);
  }

  void *GetUserData() const {
    return wl_subsurface_get_user_data(wl_subsurface_);
  }

  uint32_t GetVersion() const {
    return wl_subsurface_get_version(wl_subsurface_);
  }

  bool IsValid() const {
    return nullptr != wl_subsurface_;
  }

  bool IsNull() const {
    return nullptr == wl_subsurface_;
  }

 private:

  struct wl_subsurface *wl_subsurface_;

};

}
}

#endif // SKLAND_WAYLAND_SUBSURFACE_HPP_
