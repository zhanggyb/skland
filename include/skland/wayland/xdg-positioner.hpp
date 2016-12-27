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

#ifndef SKLAND_WAYLAND_XDG_POSITIONER_HPP_
#define SKLAND_WAYLAND_XDG_POSITIONER_HPP_

#include "xdg-shell.hpp"

namespace skland {
namespace wayland {

class XdgPopup;

struct XdgPositionerMeta;

class XdgPositioner {

  friend class XdgPopup;

  XdgPositioner(const XdgPositioner &) = delete;
  XdgPositioner &operator=(const XdgPositioner &) = delete;

 public:

  XdgPositioner();

  ~XdgPositioner();

  void Setup(const XdgShell &xdg_shell);

  void Destroy();

  void SetSize(int width, int height);

  void SetUserData(void *user_data);

  void *GetUserData() const;

  uint32_t GetVersion() const;

  bool IsValid() const;

  bool IsNull() const;

 private:

  std::unique_ptr<XdgPositionerMeta> metadata_;

};

}
}

#endif // SKLAND_WAYLAND_XDG_POSITIONER_HPP_
