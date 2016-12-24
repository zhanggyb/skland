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

#ifndef SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_TOPLEVEL_HPP_
#define SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_TOPLEVEL_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {

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

#endif // SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_TOPLEVEL_HPP_
