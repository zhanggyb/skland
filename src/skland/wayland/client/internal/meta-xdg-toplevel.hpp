//
// Created by zhanggyb on 16-12-15.
//

#ifndef SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_TOPLEVEL_HPP_
#define SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_TOPLEVEL_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {
namespace client {

struct MetaXdgToplevel {

  MetaXdgToplevel(const MetaXdgToplevel &) = delete;
  MetaXdgToplevel &operator=(const MetaXdgToplevel &) = delete;

  MetaXdgToplevel()
      : zxdg_toplevel(nullptr) {}

  ~MetaXdgToplevel() {
    if (zxdg_toplevel) zxdg_toplevel_v6_destroy(zxdg_toplevel);
  }

  struct zxdg_toplevel_v6 *zxdg_toplevel;

  static const struct zxdg_toplevel_v6_listener kListener;

  static void OnConfigure(void *data,
                          struct zxdg_toplevel_v6 *zxdg_toplevel_v6,
                          int32_t width,
                          int32_t height,
                          struct wl_array *states);

  static void OnClose(void *data,
                      struct zxdg_toplevel_v6 *zxdg_toplevel_v6);

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_TOPLEVEL_HPP_
