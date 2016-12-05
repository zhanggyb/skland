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

#ifndef SKLAND_WAYLAND_CLIENT_XDG_SHELL_HPP_
#define SKLAND_WAYLAND_CLIENT_XDG_SHELL_HPP_

#include "../protocol/xdg-shell-unstable-v6-client-protocol.h"
#include "registry.hpp"

namespace skland {
namespace wayland {
namespace client {

class XdgSurface;
class XdgPositioner;

class XdgShell {

  friend class XdgSurface;
  friend class XdgPositioner;

  XdgShell(const XdgShell &) = delete;
  XdgShell &operator=(const XdgShell &) = delete;

 public:

  XdgShell()
      : zxdg_shell_(nullptr) {}

  ~XdgShell() {
    if (zxdg_shell_) zxdg_shell_v6_destroy(zxdg_shell_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    zxdg_shell_ =
        static_cast<struct zxdg_shell_v6 *>(registry.Bind(id, &zxdg_shell_v6_interface, version));
  }

  void Destroy() {
    if (zxdg_shell_) {
      zxdg_shell_v6_destroy(zxdg_shell_);
      zxdg_shell_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    zxdg_shell_v6_set_user_data(zxdg_shell_, user_data);
  }

  void *GetUserData() const {
    return zxdg_shell_v6_get_user_data(zxdg_shell_);
  }

  uint32_t GetVersion() const {
    return zxdg_shell_v6_get_version(zxdg_shell_);
  }

  bool IsValid() const {
    return zxdg_shell_ != nullptr;
  }

  bool IsNull() const {
    return zxdg_shell_ == nullptr;
  }

  bool Equal(const void *object) const {
    return zxdg_shell_ == object;
  }

 private:

  struct zxdg_shell_v6 *zxdg_shell_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_XDG_SHELL_HPP_
