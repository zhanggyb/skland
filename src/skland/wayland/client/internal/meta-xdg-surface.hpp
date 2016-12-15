//
// Created by zhanggyb on 16-12-15.
//

#ifndef SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_SURFACE_HPP_
#define SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_SURFACE_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {
namespace client {

struct MetaXdgSurface {

  MetaXdgSurface(const MetaXdgSurface &) = delete;
  MetaXdgSurface &operator=(const MetaXdgSurface &) = delete;

  MetaXdgSurface()
      : zxdg_surface(nullptr) {}

  ~MetaXdgSurface() {
    if (zxdg_surface) zxdg_surface_v6_destroy(zxdg_surface);
  }

  struct zxdg_surface_v6 *zxdg_surface;

  static const struct zxdg_surface_v6_listener kListener;

  static void OnConfigure(void *data,
                          struct zxdg_surface_v6 *zxdg_surface_v6,
                          uint32_t serial);

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_SURFACE_HPP_
