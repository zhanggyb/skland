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

#include "internal/xdg-toplevel_private.hpp"

#include <skland/wayland/seat.hpp>
#include <skland/wayland/output.hpp>

#include "internal/xdg-surface_private.hpp"

namespace skland {
namespace wayland {

XdgToplevel::XdgToplevel() {
  p_.reset(new Private);
}

XdgToplevel::~XdgToplevel() {
}

void XdgToplevel::Setup(const XdgSurface &xdg_surface) {
  Destroy();

  p_->zxdg_toplevel = zxdg_surface_v6_get_toplevel(xdg_surface.p_->zxdg_surface);
}

void XdgToplevel::Destroy() {
  if (p_->zxdg_toplevel) {
    zxdg_toplevel_v6_destroy(p_->zxdg_toplevel);
    p_->zxdg_toplevel = nullptr;
  }
}

void XdgToplevel::AddListener(const struct zxdg_toplevel_v6_listener *listener, void *user_data) {
  zxdg_toplevel_v6_add_listener(p_->zxdg_toplevel, listener, user_data);
}

void XdgToplevel::SetParent(const XdgToplevel &parent) const {
  zxdg_toplevel_v6_set_parent(p_->zxdg_toplevel, parent.p_->zxdg_toplevel);
}

void XdgToplevel::SetTitle(const char *title) const {
  zxdg_toplevel_v6_set_title(p_->zxdg_toplevel, title);
}

void XdgToplevel::SetAppId(const char *app_id) const {
  zxdg_toplevel_v6_set_app_id(p_->zxdg_toplevel, app_id);
}

void XdgToplevel::ShowWindowMenu(const Seat &seat, uint32_t serial, int32_t x, int32_t y) const {
  zxdg_toplevel_v6_show_window_menu(p_->zxdg_toplevel, seat.wl_seat_, serial, x, y);
}

void XdgToplevel::Move(const Seat &seat, uint32_t serial) const {
  zxdg_toplevel_v6_move(p_->zxdg_toplevel, seat.wl_seat_, serial);
}

void XdgToplevel::Resize(const Seat &seat, uint32_t serial, uint32_t edges) const {
  zxdg_toplevel_v6_resize(p_->zxdg_toplevel, seat.wl_seat_, serial, edges);
}

void XdgToplevel::SetMaxSize(int32_t width, int32_t height) const {
  zxdg_toplevel_v6_set_max_size(p_->zxdg_toplevel, width, height);
}

void XdgToplevel::SetMinSize(int width, int height) const {
  zxdg_toplevel_v6_set_min_size(p_->zxdg_toplevel, width, height);
}

void XdgToplevel::SetMaximized() const {
  zxdg_toplevel_v6_set_maximized(p_->zxdg_toplevel);
}

void XdgToplevel::UnsetMaximized() const {
  zxdg_toplevel_v6_unset_maximized(p_->zxdg_toplevel);
}

void XdgToplevel::SetFullscreen(const Output &output) const {
  zxdg_toplevel_v6_set_fullscreen(p_->zxdg_toplevel, output.wl_output_);
}

void XdgToplevel::UnsetFullscreen(const Output &output) const {
  zxdg_toplevel_v6_unset_fullscreen(p_->zxdg_toplevel);
}

void XdgToplevel::SetMinimized() const {
  zxdg_toplevel_v6_set_minimized(p_->zxdg_toplevel);
}

void XdgToplevel::SetUserData(void *user_data) const {
  zxdg_toplevel_v6_set_user_data(p_->zxdg_toplevel, user_data);
}

void *XdgToplevel::GetUserData() const {
  return zxdg_toplevel_v6_get_user_data(p_->zxdg_toplevel);
}

uint32_t XdgToplevel::GetVersion() const {
  return zxdg_toplevel_v6_get_version(p_->zxdg_toplevel);
}

bool XdgToplevel::IsValid() const {
  return nullptr != p_->zxdg_toplevel;
}

}
}
