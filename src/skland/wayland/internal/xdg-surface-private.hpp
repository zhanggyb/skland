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

#ifndef SKLAND_WAYLAND_INTERNAL_XDG_SURFACE_PRIVATE_HPP_
#define SKLAND_WAYLAND_INTERNAL_XDG_SURFACE_PRIVATE_HPP_

#include <skland/wayland/xdg-surface.hpp>

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {

struct XdgSurface::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : zxdg_surface(nullptr) {}

  ~Private() {
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

#endif // SKLAND_WAYLAND_INTERNAL_XDG_SURFACE_PRIVATE_HPP_