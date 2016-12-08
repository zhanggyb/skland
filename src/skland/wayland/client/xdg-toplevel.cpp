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

#include <skland/wayland/client/xdg-toplevel.hpp>

#include <skland/core/numeric.hpp>

namespace skland {
namespace wayland {
namespace client {

const struct zxdg_toplevel_v6_listener XdgToplevel::kListener = {
    OnConfigure,
    OnClose
};

void XdgToplevel::OnConfigure(void *data,
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
        set_bit<int>(value, kStateMaskMaximized);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_FULLSCREEN: {
        set_bit<int>(value, kStateMaskFullscreen);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_RESIZING: {
        set_bit<int>(value, kStateMaskResizing);
        break;
      }
      case ZXDG_TOPLEVEL_V6_STATE_ACTIVATED: {
        set_bit<int>(value, kStateMaskActivated);
        break;
      }
      default:
        /* Unknown state */
        break;
    }
  }

  _this->configure_(width, height, value);
}

void XdgToplevel::OnClose(void *data, struct zxdg_toplevel_v6 * /* zxdg_toplevel_v6 */) {
  XdgToplevel *_this = static_cast<XdgToplevel *>(data);
  if (_this->close_)
    _this->close_();
}

}
}
}
