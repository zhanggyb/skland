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

#ifndef SKLAND_WAYLAND_CLIENT_XDG_POPUP_HPP_
#define SKLAND_WAYLAND_CLIENT_XDG_POPUP_HPP_

#include "xdg-surface.hpp"
#include "xdg-positioner.hpp"

namespace skland {
namespace wayland {
namespace client {

class XdgSurface;
class XdgPositioner;

class XdgPopup {

 public:

  XdgPopup()
      : zxdg_popup_(nullptr) {
  }

  ~XdgPopup() {
    if (zxdg_popup_) zxdg_popup_v6_destroy(zxdg_popup_);
  }

  void Setup(const XdgSurface &xdg_surface, const XdgSurface &parent, const XdgPositioner &positioner) {
    Destroy();

    zxdg_popup_ =
        zxdg_surface_v6_get_popup(xdg_surface.zxdg_surface_, parent.zxdg_surface_, positioner.zxdg_positioner_);
    zxdg_popup_v6_add_listener(zxdg_popup_, &kListener, this);
  }

  void Destroy() {
    if (zxdg_popup_) {
      zxdg_popup_v6_destroy(zxdg_popup_);
      zxdg_popup_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    zxdg_popup_v6_set_user_data(zxdg_popup_, user_data);
  }

  void *GetUserData() const {
    return zxdg_popup_v6_get_user_data(zxdg_popup_);
  }

  uint32_t GetVersion() const {
    return zxdg_popup_v6_get_version(zxdg_popup_);
  }

  bool IsValid() const {
    return zxdg_popup_ != nullptr;
  }

  bool IsNull() const {
    return zxdg_popup_ == nullptr;
  }

  DelegateRef<void(int, int, int, int)> configure() { return configure_; }

  DelegateRef<void()> done() { return done_; }

 private:

  static void OnConfigure(void *data,
                          struct zxdg_popup_v6 *zxdg_popup_v6,
                          int32_t x,
                          int32_t y,
                          int32_t width,
                          int32_t height);

  static void OnPopupDone(void *data,
                          struct zxdg_popup_v6 *zxdg_popup_v6);

  static const struct zxdg_popup_v6_listener kListener;

  struct zxdg_popup_v6 *zxdg_popup_;

  Delegate<void(int, int, int, int)> configure_;

  Delegate<void()> done_;
};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_XDG_POPUP_HPP_
