//
// Created by zhanggyb on 16-12-15.
//

#include "meta-xdg-shell.hpp"
#include <skland/wayland/client/xdg-shell.hpp>

namespace skland {
namespace wayland {
namespace client {

const struct zxdg_shell_v6_listener MetaXdgShell::kListener = {
    OnPing
};

void MetaXdgShell::OnPing(void *data, struct zxdg_shell_v6 *zxdg_shell_v6, uint32_t serial) {
  XdgShell *_this = static_cast<XdgShell *>(data);
  if (_this->ping_) {
    _this->ping_(serial);
  }
}

}
}
}