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

#ifndef SKLAND_WAYLAND_XDG_POPUP_HPP_
#define SKLAND_WAYLAND_XDG_POPUP_HPP_

#include "xdg-surface.hpp"
#include "xdg-positioner.hpp"

// Forward declaration
struct zxdg_popup_v6_listener;

namespace skland {
namespace wayland {

class XdgPopup {

 public:

  XdgPopup();

  ~XdgPopup();

  void Setup(const XdgSurface &xdg_surface, const XdgSurface &parent, const XdgPositioner &positioner);

  void Destroy();

  void AddListener(const struct zxdg_popup_v6_listener *listener, void *user_data);

  void SetUserData(void *user_data);

  void *GetUserData() const;

  uint32_t GetVersion() const;

  bool IsValid() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

}
}

#endif // SKLAND_WAYLAND_XDG_POPUP_HPP_
