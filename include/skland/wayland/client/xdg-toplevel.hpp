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

#include "xdg-surface.hpp"

namespace skland {
namespace wayland {
namespace client {

class Seat;
class Output;
struct MetaXdgToplevel;

class XdgToplevel {

  friend struct MetaXdgToplevel;

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

  XdgToplevel();

  ~XdgToplevel();

  void Setup(const XdgSurface &xdg_surface);

  void Destroy();

  void SetParent(const XdgToplevel &parent) const;

  void SetTitle(const char *title) const;

  void SetAppId(const char *app_id) const;

  void ShowWindowMenu(const Seat &seat, uint32_t serial, int32_t x, int32_t y) const;

  void Move(const Seat &seat, uint32_t serial) const;

  void Resize(const Seat &seat, uint32_t serial, uint32_t edges) const;

  void SetMaxSize(int32_t width, int32_t height) const;

  void SetMinSize(int width, int height) const;

  void SetMaximized() const;

  void UnsetMaximized() const;

  void SetFullscreen(const Output &output) const;

  void UnsetFullscreen(const Output &output) const;

  void SetMinimized() const;

  void SetUserData(void *user_data) const;

  void *GetUserData() const;

  uint32_t GetVersion() const;

  bool IsValid() const;

  bool IsNull() const;

  DelegateRef<void(int, int, int)> configure() { return configure_; }

  DelegateRef<void()> close() { return close_; }

 private:

  MetaXdgToplevel *metadata_;

  Delegate<void(int, int, int)> configure_;
  Delegate<void()> close_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_XDG_TOPLEVEL_HPP_
