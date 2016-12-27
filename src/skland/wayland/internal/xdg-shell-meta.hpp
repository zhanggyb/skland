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

#ifndef SKLAND_WAYLAND_INTERNAL_XDG_SHELL_META_HPP_
#define SKLAND_WAYLAND_INTERNAL_XDG_SHELL_META_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {

struct XdgShellMeta {

  XdgShellMeta(const XdgShellMeta &) = delete;
  XdgShellMeta &operator=(const XdgShellMeta &) = delete;

  XdgShellMeta()
      : zxdg_shell(nullptr) {}

  ~XdgShellMeta() {
    if (zxdg_shell) zxdg_shell_v6_destroy(zxdg_shell);
  }

  struct zxdg_shell_v6 *zxdg_shell;

  static void OnPing(void *data,
                     struct zxdg_shell_v6 *zxdg_shell_v6,
                     uint32_t serial);

  static const struct zxdg_shell_v6_listener kListener;

};

}
}

#endif // SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_SHELL_HPP_
