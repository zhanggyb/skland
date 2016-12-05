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

#ifndef SKLAND_WAYLAND_CLIENT_XDG_TOPLEVEL_HPP_
#define SKLAND_WAYLAND_CLIENT_XDG_TOPLEVEL_HPP_

#include "seat.hpp"
#include "xdg-surface.hpp"

namespace skland {
namespace wayland {
namespace client {

class XdgToplevel {

  XdgToplevel(const XdgToplevel &) = delete;
  XdgToplevel &operator=(const XdgToplevel &) = delete;

 public:

  enum StatesMask {
    /**
     * the surface is maximized
     */
        kStateMaskMaximized = 0x1, /* 1 */
    /**
     * the surface is fullscreen
     */
        kStateMaskFullscreen = 0x1 << 1,  /* 2 */
    /**
     * the surface is being resized
     */
        kStateMaskResizing = 0x1 << 2, /* 4 */
    /**
     * the surface is now activated
     */
        kStateMaskActivated = 0x1 << 3, /* 8 */
  };

  XdgToplevel()
      : zxdg_toplevel_(nullptr) {
  }

  ~XdgToplevel() {
    if (zxdg_toplevel_) zxdg_toplevel_v6_destroy(zxdg_toplevel_);
  }

  void Setup(const XdgSurface &xdg_surface) {
    Destroy();

    zxdg_toplevel_ = zxdg_surface_v6_get_toplevel(xdg_surface.zxdg_surface_);
    zxdg_toplevel_v6_add_listener(zxdg_toplevel_, &kListener, this);
  }

  void Destroy() {
    if (zxdg_toplevel_) {
      zxdg_toplevel_v6_destroy(zxdg_toplevel_);
      zxdg_toplevel_ = nullptr;
    }
  }

  void Move(const Seat &seat, uint32_t serial) const {
    zxdg_toplevel_v6_move(zxdg_toplevel_, seat.wl_seat_, serial);
  }

  void Resize(const Seat &seat, uint32_t serial, uint32_t edges) const {
    zxdg_toplevel_v6_resize(zxdg_toplevel_, seat.wl_seat_, serial, edges);
  }

  bool IsValid() const {
    return zxdg_toplevel_ != nullptr;
  }

  bool IsNull() const {
    return zxdg_toplevel_ == nullptr;
  }

  DelegateRef<void(int, int, int)> configure() { return configure_; }

  DelegateRef<void()> close() { return close_; }

 private:

  /**
 * suggest a surface change
 *
 * This configure event asks the client to resize its toplevel
 * surface or to change its state. The configured state should not
 * be applied immediately. See xdg_surface.configure for details.
 *
 * The width and height arguments specify a hint to the window
 * about how its surface should be resized in window geometry
 * coordinates. See set_window_geometry.
 *
 * If the width or height arguments are zero, it means the client
 * should decide its own window dimension. This may happen when the
 * compositor need to configure the state of the surface but
 * doesn't have any information about any previous or expected
 * dimension.
 *
 * The states listed in the event specify how the width/height
 * arguments should be interpreted, and possibly how it should be
 * drawn.
 *
 * Clients must send an ack_configure in response to this event.
 * See xdg_surface.configure and xdg_surface.ack_configure for
 * details.
 */
  static void OnConfigure(void *data,
                          struct zxdg_toplevel_v6 *zxdg_toplevel_v6,
                          int32_t width,
                          int32_t height,
                          struct wl_array *states);

  static void OnClose(void *data,
                      struct zxdg_toplevel_v6 *zxdg_toplevel_v6);

  static const struct zxdg_toplevel_v6_listener kListener;

  struct zxdg_toplevel_v6 *zxdg_toplevel_;

  Delegate<void(int, int, int)> configure_;
  Delegate<void()> close_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_XDG_TOPLEVEL_HPP_
