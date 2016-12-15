//
// Created by zhanggyb on 16-12-15.
//

#include "meta-xdg-surface.hpp"

#include <skland/wayland/client/xdg-surface.hpp>

namespace skland {
namespace wayland {
namespace client {

const struct zxdg_surface_v6_listener MetaXdgSurface::kListener = {
    OnConfigure
};

void MetaXdgSurface::OnConfigure(void *data, struct zxdg_surface_v6 * /* zxdg_surface_v6 */, uint32_t serial) {
  XdgSurface *_this = static_cast<XdgSurface *>(data);
  if (_this->configure_)
    _this->configure_(serial);
}

}
}
}