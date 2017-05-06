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

#include <skland/wayland/xdg-surface.hpp>
#include <skland/wayland/surface.hpp>

#include "internal/xdg-surface_private.hpp"
#include "internal/xdg-shell_private.hpp"

namespace skland {
namespace wayland {

XdgSurface::XdgSurface() {
  p_.reset(new Private);
}

XdgSurface::~XdgSurface() {
}

void XdgSurface::Setup(const XdgShell &xdg_shell, const Surface &surface) {
  Destroy();

  p_->zxdg_surface = zxdg_shell_v6_get_xdg_surface(xdg_shell.p_->zxdg_shell, surface.wl_surface_);
}

void XdgSurface::Destroy() {
  if (p_->zxdg_surface) {
    zxdg_surface_v6_destroy(p_->zxdg_surface);
    p_->zxdg_surface = nullptr;
  }
}

void XdgSurface::AddListener(const struct zxdg_surface_v6_listener *listener, void *user_data) {
  zxdg_surface_v6_add_listener(p_->zxdg_surface, listener, user_data);
}

void XdgSurface::SetWindowGeometry(int x, int y, int width, int height) const {
  zxdg_surface_v6_set_window_geometry(p_->zxdg_surface, x, y, width, height);
}

void XdgSurface::AckConfigure(uint32_t serial) const {
  zxdg_surface_v6_ack_configure(p_->zxdg_surface, serial);
}

void XdgSurface::SetUserData(void *user_data) {
  zxdg_surface_v6_set_user_data(p_->zxdg_surface, user_data);
}

void *XdgSurface::GetUserData() {
  return zxdg_surface_v6_get_user_data(p_->zxdg_surface);
}

uint32_t XdgSurface::GetVersion() const {
  return zxdg_surface_v6_get_version(p_->zxdg_surface);
}

bool XdgSurface::IsValid() const {
  return nullptr != p_->zxdg_surface;
}

}
}
