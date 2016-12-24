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

#include <skland/wayland/shell-surface.hpp>

namespace skland {
namespace wayland {

const wl_shell_surface_listener ShellSurface::kListener = {
    OnPing,
    OnConfigure,
    OnPopupDone
};

void ShellSurface::OnPing(void *data, struct wl_shell_surface *wl_shell_surface, uint32_t serial) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->ping_) _this->ping_(serial);
}

void ShellSurface::OnConfigure(void *data,
                               struct wl_shell_surface *wl_shell_surface,
                               uint32_t edges,
                               int32_t width,
                               int32_t height) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->configure_) _this->configure_(edges, width, height);
}

void ShellSurface::OnPopupDone(void *data, struct wl_shell_surface *wl_shell_surface) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->done_) _this->done_();
}

}
}
