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

#include "internal/xdg-surface-meta.hpp"
#include "internal/xdg-shell-meta.hpp"

namespace skland {
namespace wayland {

XdgSurface::XdgSurface() {
  metadata_.reset(new XdgSurfaceMeta);
}

XdgSurface::~XdgSurface() {
}

void XdgSurface::Setup(const XdgShell &xdg_shell, const Surface &surface) {
  Destroy();

  metadata_->zxdg_surface = zxdg_shell_v6_get_xdg_surface(xdg_shell.metadata_->zxdg_shell, surface.wl_surface_);
  zxdg_surface_v6_add_listener(metadata_->zxdg_surface, &XdgSurfaceMeta::kListener, this);
}

void XdgSurface::Destroy() {
  if (metadata_->zxdg_surface) {
    zxdg_surface_v6_destroy(metadata_->zxdg_surface);
    metadata_->zxdg_surface = nullptr;
  }
}

void XdgSurface::SetWindowGeometry(int x, int y, int width, int height) {
  zxdg_surface_v6_set_window_geometry(metadata_->zxdg_surface, x, y, width, height);
}

void XdgSurface::AckConfigure(uint32_t serial) {
  zxdg_surface_v6_ack_configure(metadata_->zxdg_surface, serial);
}

void XdgSurface::SetUserData(void *user_data) {
  zxdg_surface_v6_set_user_data(metadata_->zxdg_surface, user_data);
}

void *XdgSurface::GetUserData() {
  return zxdg_surface_v6_get_user_data(metadata_->zxdg_surface);
}

uint32_t XdgSurface::GetVersion() const {
  return zxdg_surface_v6_get_version(metadata_->zxdg_surface);
}

bool XdgSurface::IsValid() const {
  return nullptr != metadata_->zxdg_surface;
}

bool XdgSurface::IsNull() const {
  return nullptr == metadata_->zxdg_surface;
}

}
}
