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

#include "internal/meta-xdg-positioner.hpp"
#include "internal/meta-xdg-shell.hpp"

namespace skland {
namespace wayland {

XdgPositioner::XdgPositioner()
    : metadata_(nullptr) {
  metadata_ = new MetaXdgPositioner;
}

XdgPositioner::~XdgPositioner() {
  delete metadata_;
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

bool XdgPositioner::IsNull() const {
  return nullptr == metadata_->zxdg_positioner;
}

}
}
