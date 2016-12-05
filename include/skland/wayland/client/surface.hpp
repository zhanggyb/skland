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

#ifndef SKLAND_WAYLAND_CLIENT_SURFACE_HPP_
#define SKLAND_WAYLAND_CLIENT_SURFACE_HPP_

#include "compositor.hpp"
#include "buffer.hpp"
#include "region.hpp"

namespace skland {
namespace wayland {
namespace client {

class ShellSurface;
class SubSurface;
class XdgSurface;
class Callback;
class Pointer;

class Surface {

  friend class ShellSurface;
  friend class SubSurface;
  friend class XdgSurface;
  friend class Callback;
  friend class Pointer;

  Surface(const Surface &) = delete;

  Surface &operator=(const Surface &) = delete;

 public:

  Surface()
      : wl_surface_(nullptr) {
  }

  ~Surface() {
    if (wl_surface_) wl_surface_destroy(wl_surface_);
  }

  void Setup(const Compositor &compositor) {
    Destroy();

    wl_surface_ = wl_compositor_create_surface(compositor.wl_compositor_);
    wl_surface_add_listener(wl_surface_, &kListener, this);
  }

  void Destroy() {
    if (wl_surface_) {
      wl_surface_destroy(wl_surface_);
      wl_surface_ = nullptr;
    }
  }

  void Attach(struct wl_buffer *buffer, int32_t x, int32_t y) const {
    wl_surface_attach(wl_surface_, buffer, x, y);
  }

  void Attach(const Buffer &buffer, int32_t x, int32_t y) const {
    wl_surface_attach(wl_surface_, buffer.wl_buffer_, x, y);
  }

  void Commit() const {
    wl_surface_commit(wl_surface_);
  }

  void Damage(int32_t x, int32_t y, int32_t width, int32_t height) const {
    wl_surface_damage(wl_surface_, x, y, width, height);
  }

  void SetInputRegion(const Region &region) const {
    wl_surface_set_input_region(wl_surface_, region.wl_region_);
  }

  void SetOpaqueRegion(const Region &region) const {
    wl_surface_set_opaque_region(wl_surface_, region.wl_region_);
  }

  void SetUserData(void *user_data) {
    wl_surface_set_user_data(wl_surface_, user_data);
  }

  void *GetUserData() const {
    return wl_surface_get_user_data(wl_surface_);
  }

  uint32_t GetVersion() const {
    return wl_surface_get_version(wl_surface_);
  }

  bool IsValid() const {
    return wl_surface_ != nullptr;
  }

  bool IsNull() const {
    return wl_surface_ == nullptr;
  }

  DelegateRef<void(struct wl_output *)> enter() {
    return enter_;
  }

  DelegateRef<void(struct wl_output *)> leave() {
    return leave_;
  }

 private:

  static void OnEnter(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static void OnLeave(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static const struct wl_surface_listener kListener;

  struct wl_surface *wl_surface_;

  Delegate<void(struct wl_output *)> enter_;
  Delegate<void(struct wl_output *)> leave_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_SURFACE_HPP_
