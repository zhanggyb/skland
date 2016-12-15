//
// Created by zhanggyb on 16-12-15.
//

#include "meta-xdg-toplevel.hpp"

#include <skland/wayland/client/xdg-toplevel.hpp>
#include <skland/core/numeric.hpp>

namespace skland {
namespace wayland {
namespace client {

const struct zxdg_toplevel_v6_listener MetaXdgToplevel::kListener = {
    OnConfigure,
    OnClose
};

void MetaXdgToplevel::OnConfigure(void *data,
                                  struct zxdg_toplevel_v6 * /* zxdg_toplevel_v6 */,
                                  int32_t width,
                                  int32_t height,
                                  struct wl_array *states) {
  XdgToplevel *_this = static_cast<XdgToplevel *>(data);

  if (!_this->configure_) return;

  void *p = nullptr;
  int value = 0;

  wl_array_for_each(p, states) {
    uint32_t state = *((uint32_t *) p);
    switch (state) {
      case ZXDG_TOPLEVEL_V6_STATE_MAXIMIZED: {
        set_bit<int>(value, XdgToplevel::kStateMaskMaximized);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_FULLSCREEN: {
        set_bit<int>(value, XdgToplevel::kStateMaskFullscreen);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_RESIZING: {
        set_bit<int>(value, XdgToplevel::kStateMaskResizing);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_ACTIVATED: {
        set_bit<int>(value, XdgToplevel::kStateMaskActivated);
        break;
      }
      default:
        /* Unknown state */
        break;
    }
  }

  _this->configure_(width, height, value);
}

void MetaXdgToplevel::OnClose(void *data, struct zxdg_toplevel_v6 * /* zxdg_toplevel_v6 */) {
  XdgToplevel *_this = static_cast<XdgToplevel *>(data);
  if (_this->close_)
    _this->close_();
}

}
}
}