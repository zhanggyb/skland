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

#ifndef SKLAND_WAYLAND_SHELL_SURFACE_HPP_
#define SKLAND_WAYLAND_SHELL_SURFACE_HPP_

#include "shell.hpp"
#include "surface.hpp"

namespace skland {
namespace wayland {

class ShellSurface {

 public:

  ShellSurface()
      : wl_shell_surface_(nullptr) {}

  ~ShellSurface() {
    if (wl_shell_surface_) wl_shell_surface_destroy(wl_shell_surface_);
  }

  void Setup(const Shell &shell, const Surface &surface) {
    Destroy();
    wl_shell_surface_ = wl_shell_get_shell_surface(shell.wl_shell_, surface.wl_surface_);
  }

  void Destroy() {
    if (wl_shell_surface_) {
      wl_shell_surface_destroy(wl_shell_surface_);
      wl_shell_surface_ = nullptr;
    }
  }

  void AddListener(const struct wl_shell_surface_listener *listener, void *user_data) {
    wl_shell_surface_add_listener(wl_shell_surface_, listener, user_data);
  }

  void SetUserData(void *user_data) {
    wl_shell_surface_set_user_data(wl_shell_surface_, user_data);
  }

  void *GetUserData() const {
    return wl_shell_surface_get_user_data(wl_shell_surface_);
  }

  uint32_t GetVersion() const {
    return wl_shell_surface_get_version(wl_shell_surface_);
  }

  bool IsValid() {
    return nullptr != wl_shell_surface_;
  }

 private:

  struct wl_shell_surface *wl_shell_surface_;

};

}
}

#endif // SKLAND_WAYLAND_SHELL_SURFACE_HPP_
