//
// Created by zhanggyb on 16-12-15.
//

#ifndef SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_POSITIONER_HPP_
#define SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_POSITIONER_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {
namespace client {

struct MetaXdgPositioner {

  MetaXdgPositioner(const MetaXdgPositioner &) = delete;
  MetaXdgPositioner &operator=(const MetaXdgPositioner &) = delete;

  MetaXdgPositioner()
      : zxdg_positioner(nullptr) {}

  ~MetaXdgPositioner() {
    if (zxdg_positioner) zxdg_positioner_v6_destroy(zxdg_positioner);
  }

  struct zxdg_positioner_v6 *zxdg_positioner;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_POSITIONER_HPP_
