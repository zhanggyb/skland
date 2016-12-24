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

#include "meta-xdg-shell.hpp"
#include <skland/wayland/xdg-shell.hpp>

namespace skland {
namespace wayland {

const struct zxdg_shell_v6_listener MetaXdgShell::kListener = {
    OnPing
};

void MetaXdgShell::OnPing(void *data, struct zxdg_shell_v6 *zxdg_shell_v6, uint32_t serial) {
  XdgShell *_this = static_cast<XdgShell *>(data);
  if (_this->ping_) {
    _this->ping_(serial);
  }
}

}
}
