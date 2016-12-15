//
// Created by zhanggyb on 16-12-15.
//

#ifndef SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_POPUP_HPP_
#define SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_POPUP_HPP_

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {
namespace wayland {
namespace client {

struct MetaXdgPopup {

  MetaXdgPopup(const MetaXdgPopup &) = delete;
  MetaXdgPopup &operator=(const MetaXdgPopup &) = delete;

  inline MetaXdgPopup()
      : zxdg_popup(nullptr) {}

  inline ~MetaXdgPopup() {
    if (zxdg_popup) zxdg_popup_v6_destroy(zxdg_popup);
  }

  struct zxdg_popup_v6 *zxdg_popup;

  static const struct zxdg_popup_v6_listener kListener;

  static void OnConfigure(void *data,
                          struct zxdg_popup_v6 *zxdg_popup_v6,
                          int32_t x,
                          int32_t y,
                          int32_t width,
                          int32_t height);

  static void OnPopupDone(void *data,
                          struct zxdg_popup_v6 *zxdg_popup_v6);

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_INTERNAL_META_XDG_POPUP_HPP_
