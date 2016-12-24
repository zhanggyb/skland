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

#ifndef SKLAND_WAYLAND_XDG_SHELL_HPP_
#define SKLAND_WAYLAND_XDG_SHELL_HPP_

#include "registry.hpp"

namespace skland {
namespace wayland {

class XdgSurface;
class XdgPositioner;

struct MetaXdgShell;

class XdgShell {

  friend class XdgSurface;
  friend class XdgPositioner;
  friend struct MetaXdgShell;

  XdgShell(const XdgShell &) = delete;
  XdgShell &operator=(const XdgShell &) = delete;

 public:

  static const struct wl_interface *GetInterface();

  XdgShell();

  ~XdgShell();

  void Setup(const Registry &registry, uint32_t id, uint32_t version);

  void Destroy();

  void Pong(uint32_t serial) const;

  void SetUserData(void *user_data);

  void *GetUserData() const;

  uint32_t GetVersion() const;

  bool IsValid() const;

  bool IsNull() const;

  bool Equal(const void *object) const;

  DelegateRef<void(uint32_t)> ping() {
    return ping_;
  }

 private:

  MetaXdgShell *metadata_;

  Delegate<void(uint32_t)> ping_;

};

}
}

#endif // SKLAND_WAYLAND_XDG_SHELL_HPP_
