//
// Created by zhanggyb on 16-12-15.
//

#include <skland/wayland/client/xdg-positioner.hpp>
#include <skland/wayland/client/xdg-shell.hpp>

#include "internal/meta-xdg-positioner.hpp"
#include "internal/meta-xdg-shell.hpp"

namespace skland {
namespace wayland {
namespace client {

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
}
