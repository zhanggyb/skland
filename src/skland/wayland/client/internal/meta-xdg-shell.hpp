//
// Created by zhanggyb on 16-12-15.
//

#ifndef SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_SHELL_HPP_
#define SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_SHELL_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {
namespace client {

struct MetaXdgShell {

  MetaXdgShell(const MetaXdgShell &) = delete;
  MetaXdgShell &operator=(const MetaXdgShell &) = delete;

  MetaXdgShell()
      : zxdg_shell(nullptr) {}

  ~MetaXdgShell() {
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
}

#endif // SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_SHELL_HPP_
