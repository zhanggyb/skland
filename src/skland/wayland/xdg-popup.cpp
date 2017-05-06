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

#include "internal/xdg-popup_private.hpp"

#include "internal/xdg-surface_private.hpp"
#include "internal/xdg-positioner_private.hpp"

namespace skland {
namespace wayland {

XdgPopup::XdgPopup() {
  p_.reset(new Private);
}

XdgPopup::~XdgPopup() {

}

void XdgPopup::Setup(const XdgSurface &xdg_surface, const XdgSurface &parent, const XdgPositioner &positioner) {
  Destroy();

  p_->zxdg_popup =
      zxdg_surface_v6_get_popup(xdg_surface.p_->zxdg_surface,
                                parent.p_->zxdg_surface,
                                positioner.p_->zxdg_positioner);
  zxdg_popup_v6_add_listener(p_->zxdg_popup, &XdgPopup::Private::kListener, this);
}

void XdgPopup::Destroy() {
  if (p_->zxdg_popup) {
    zxdg_popup_v6_destroy(p_->zxdg_popup);
    p_->zxdg_popup = nullptr;
  }
}

void XdgPopup::SetUserData(void *user_data) {
  zxdg_popup_v6_set_user_data(p_->zxdg_popup, user_data);
}

void *XdgPopup::GetUserData() const {
  return zxdg_popup_v6_get_user_data(p_->zxdg_popup);
}

uint32_t XdgPopup::GetVersion() const {
  return zxdg_popup_v6_get_version(p_->zxdg_popup);
}

bool XdgPopup::IsValid() const {
  return nullptr != p_->zxdg_popup;
}

}
}

