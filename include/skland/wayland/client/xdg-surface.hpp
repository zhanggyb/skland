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

#ifndef SKLAND_WAYLAND_CLIENT_XDG_SURFACE_HPP_
#define SKLAND_WAYLAND_CLIENT_XDG_SURFACE_HPP_

#include "xdg-shell.hpp"
#include "surface.hpp"

namespace skland {
namespace wayland {
namespace client {

class XdgPopup;
class XdgToplevel;

class XdgSurface {

  friend class XdgPopup;
  friend class XdgToplevel;

  XdgSurface(const XdgSurface &) = delete;
  XdgSurface &operator=(const XdgSurface &) = delete;

 public:

  XdgSurface()
      : zxdg_surface_(nullptr) {}

  ~XdgSurface() {
    if (zxdg_surface_ != nullptr) zxdg_surface_v6_destroy(zxdg_surface_);
  }

  void Setup(const XdgShell &xdg_shell, const Surface &surface) {
    Destroy();

    zxdg_surface_ = zxdg_shell_v6_get_xdg_surface(xdg_shell.zxdg_shell_, surface.wl_surface_);
    zxdg_surface_v6_add_listener(zxdg_surface_, &kListener, this);
  }

  void Destroy() {
    if (zxdg_surface_) {
      zxdg_surface_v6_destroy(zxdg_surface_);
      zxdg_surface_ = nullptr;
    }
  }

  /**
 *
 * The window geometry of a surface is its "visible bounds" from the
 * user's perspective. Client-side decorations often have invisible
 * portions like drop-shadows which should be ignored for the
 * purposes of aligning, placing and constraining windows.
 *
 * The window geometry is double buffered, and will be applied at the
 * time wl_surface.commit of the corresponding wl_surface is called.
 *
 * Once the window geometry of the surface is set, it is not possible to
 * unset it, and it will remain the same until set_window_geometry is
 * called again, even if a new subsurface or buffer is attached.
 *
 * If never set, the value is the full bounds of the surface,
 * including any subsurfaces. This updates dynamically on every
 * commit. This unset is meant for extremely simple clients.
 *
 * The arguments are given in the surface-local coordinate space of
 * the wl_surface associated with this xdg_surface.
 *
 * The width and height must be greater than zero. Setting an invalid size
 * will raise an error. When applied, the effective window geometry will be
 * the set window geometry clamped to the bounding rectangle of the
 * combined geometry of the surface of the xdg_surface and the associated
 * subsurfaces.
 */
  void SetWindowGeometry(int x, int y, int width, int height) {
    zxdg_surface_v6_set_window_geometry(zxdg_surface_, x, y, width, height);
  }

  void AckConfigure(uint32_t serial) {
    zxdg_surface_v6_ack_configure(zxdg_surface_, serial);
  }

  void SetUserData(void *user_data) {
    zxdg_surface_v6_set_user_data(zxdg_surface_, user_data);
  }

  void *GetUserData() {
    return zxdg_surface_v6_get_user_data(zxdg_surface_);
  }

  uint32_t GetVersion() const {
    return zxdg_surface_v6_get_version(zxdg_surface_);
  }

  bool IsValid() const {
    return zxdg_surface_ != nullptr;
  }

  bool IsNull() const {
    return zxdg_surface_ == nullptr;
  }

  DelegateRef<void(uint32_t)> configure() { return configure_; }

 private:

  static void OnConfigure(void *data,
                          struct zxdg_surface_v6 *zxdg_surface_v6,
                          uint32_t serial);

  static const struct zxdg_surface_v6_listener kListener;

  struct zxdg_surface_v6 *zxdg_surface_;

  Delegate<void(uint32_t)> configure_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_XDG_SURFACE_HPP_
