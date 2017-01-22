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

#ifndef SKLAND_WAYLAND_XDG_SURFACE_HPP_
#define SKLAND_WAYLAND_XDG_SURFACE_HPP_

#include "xdg-shell.hpp"

namespace skland {
namespace wayland {

class Surface;
class XdgPopup;
class XdgToplevel;

struct XdgSurfaceMeta;

class XdgSurface {

  friend class XdgPopup;
  friend class XdgToplevel;
  friend struct XdgSurfaceMeta;

  XdgSurface(const XdgSurface &) = delete;
  XdgSurface &operator=(const XdgSurface &) = delete;

 public:

  XdgSurface();

  ~XdgSurface();

  void Setup(const XdgShell &xdg_shell, const Surface &surface);

  void Destroy();

  void SetWindowGeometry(int x, int y, int width, int height);

  void AckConfigure(uint32_t serial);

  void SetUserData(void *user_data);

  void *GetUserData();

  uint32_t GetVersion() const;

  bool IsValid() const;

  DelegateRef<void(uint32_t)> configure() { return configure_; }

 private:

  std::unique_ptr<XdgSurfaceMeta> metadata_;

  Delegate<void(uint32_t)> configure_;

};

}
}

#endif // SKLAND_WAYLAND_XDG_SURFACE_HPP_
