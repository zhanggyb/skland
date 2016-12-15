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

#ifndef SKLAND_WAYLAND_CLIENT_XDG_POPUP_HPP_
#define SKLAND_WAYLAND_CLIENT_XDG_POPUP_HPP_

#include "xdg-surface.hpp"
#include "xdg-positioner.hpp"

namespace skland {
namespace wayland {
namespace client {

struct MetaXdgPopup;

class XdgPopup {

  friend struct MetaXdgPopup;

 public:

  XdgPopup();

  ~XdgPopup();

  void Setup(const XdgSurface &xdg_surface, const XdgSurface &parent, const XdgPositioner &positioner);

  void Destroy();

  void SetUserData(void *user_data);

  void *GetUserData() const;

  uint32_t GetVersion() const;

  bool IsValid() const;

  bool IsNull() const;

  DelegateRef<void(int, int, int, int)> configure() { return configure_; }

  DelegateRef<void()> done() { return done_; }

 private:

  MetaXdgPopup *metadata_;

  Delegate<void(int, int, int, int)> configure_;

  Delegate<void()> done_;
};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_XDG_POPUP_HPP_
