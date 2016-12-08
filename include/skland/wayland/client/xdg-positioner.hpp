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

#ifndef SKLAND_WAYLAND_CLIENT_XDG_POSITIONER_HPP_
#define SKLAND_WAYLAND_CLIENT_XDG_POSITIONER_HPP_

#include "xdg-shell.hpp"

namespace skland {
namespace wayland {
namespace client {

class XdgPopup;

class XdgPositioner {

  friend class XdgPopup;

  XdgPositioner(const XdgPositioner &) = delete;
  XdgPositioner &operator=(const XdgPositioner &) = delete;

 public:

  XdgPositioner()
      : zxdg_positioner_(nullptr) {
  }

  ~XdgPositioner() {
    if (zxdg_positioner_) zxdg_positioner_v6_destroy(zxdg_positioner_);
  }

  void Setup(const XdgShell &xdg_shell) {
    Destroy();

    zxdg_positioner_ = zxdg_shell_v6_create_positioner(xdg_shell.zxdg_shell_);
  }

  void Destroy() {
    if (zxdg_positioner_) {
      zxdg_positioner_v6_destroy(zxdg_positioner_);
      zxdg_positioner_ = nullptr;
    }
  }

  void SetSize(int width, int height) {
    zxdg_positioner_v6_set_size(zxdg_positioner_, width, height);
  }

  void SetUserData(void *user_data) {
    zxdg_positioner_v6_set_user_data(zxdg_positioner_, user_data);
  }

  void *GetUserData() const {
    return zxdg_positioner_v6_get_user_data(zxdg_positioner_);
  }

  uint32_t GetVersion() const {
    return zxdg_positioner_v6_get_version(zxdg_positioner_);
  }

  bool IsValid() const {
    return nullptr != zxdg_positioner_;
  }

  bool IsNull() const {
    return nullptr == zxdg_positioner_;
  }

 private:

  struct zxdg_positioner_v6 *zxdg_positioner_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_XDG_POSITIONER_HPP_
