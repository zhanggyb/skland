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

#include <skland/wayland/xdg-positioner.hpp>

#include "internal/xdg-positioner-meta.hpp"
#include "internal/xdg-shell-meta.hpp"

namespace skland {
namespace wayland {

XdgPositioner::XdgPositioner() {
  metadata_.reset(new XdgPositionerMeta);
}

XdgPositioner::~XdgPositioner() {

}

void XdgPositioner::Setup(const XdgShell &xdg_shell) {
  Destroy();

  metadata_->zxdg_positioner = zxdg_shell_v6_create_positioner(xdg_shell.metadata_->zxdg_shell);
}

void XdgPositioner::Destroy() {
  if (metadata_->zxdg_positioner) {
    zxdg_positioner_v6_destroy(metadata_->zxdg_positioner);
    metadata_->zxdg_positioner = nullptr;
  }
}

void XdgPositioner::SetSize(int width, int height) {
  zxdg_positioner_v6_set_size(metadata_->zxdg_positioner, width, height);
}

void XdgPositioner::SetAnchorRect(int32_t x, int32_t y, int32_t width, int32_t height) {
  zxdg_positioner_v6_set_anchor_rect(metadata_->zxdg_positioner, x, y, width, height);
}

void XdgPositioner::SetAnchor(uint32_t anchor) {
  zxdg_positioner_v6_set_anchor(metadata_->zxdg_positioner, anchor);
}

void XdgPositioner::SetGravity(uint32_t gravity) {
  zxdg_positioner_v6_set_gravity(metadata_->zxdg_positioner, gravity);
}

void XdgPositioner::SetConstraintAdjustment(uint32_t constraint_adjustment) {
  zxdg_positioner_v6_set_constraint_adjustment(metadata_->zxdg_positioner, constraint_adjustment);
}

void XdgPositioner::SetOffset(int32_t x, int32_t y) {
  zxdg_positioner_v6_set_offset(metadata_->zxdg_positioner, x, y);
}

void XdgPositioner::SetUserData(void *user_data) {
  zxdg_positioner_v6_set_user_data(metadata_->zxdg_positioner, user_data);
}

void *XdgPositioner::GetUserData() const {
  return zxdg_positioner_v6_get_user_data(metadata_->zxdg_positioner);
}

uint32_t XdgPositioner::GetVersion() const {
  return zxdg_positioner_v6_get_version(metadata_->zxdg_positioner);
}

bool XdgPositioner::IsValid() const {
  return nullptr != metadata_->zxdg_positioner;
}

} // namespace wayland
} // namespace skland
