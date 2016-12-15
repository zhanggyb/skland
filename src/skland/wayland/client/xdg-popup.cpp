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

#include <skland/wayland/client/xdg-popup.hpp>

#include "internal/meta-xdg-popup.hpp"
#include "internal/meta-xdg-surface.hpp"
#include "internal/meta-xdg-positioner.hpp"

namespace skland {
namespace wayland {
namespace client {

XdgPopup::XdgPopup()
    : metadata_(nullptr) {
  metadata_ = new MetaXdgPopup;
}

XdgPopup::~XdgPopup() {
  delete metadata_;
}

void XdgPopup::Setup(const XdgSurface &xdg_surface, const XdgSurface &parent, const XdgPositioner &positioner) {
  Destroy();

  metadata_->zxdg_popup =
      zxdg_surface_v6_get_popup(xdg_surface.metadata_->zxdg_surface,
                                parent.metadata_->zxdg_surface,
                                positioner.metadata_->zxdg_positioner);
  zxdg_popup_v6_add_listener(metadata_->zxdg_popup, &MetaXdgPopup::kListener, this);
}

void XdgPopup::Destroy() {
  if (metadata_->zxdg_popup) {
    zxdg_popup_v6_destroy(metadata_->zxdg_popup);
    metadata_->zxdg_popup = nullptr;
  }
}

void XdgPopup::SetUserData(void *user_data) {
  zxdg_popup_v6_set_user_data(metadata_->zxdg_popup, user_data);
}

void *XdgPopup::GetUserData() const {
  return zxdg_popup_v6_get_user_data(metadata_->zxdg_popup);
}

uint32_t XdgPopup::GetVersion() const {
  return zxdg_popup_v6_get_version(metadata_->zxdg_popup);
}

bool XdgPopup::IsValid() const {
  return nullptr != metadata_->zxdg_popup;
}

bool XdgPopup::IsNull() const {
  return nullptr == metadata_->zxdg_popup;
}

}
}
}
