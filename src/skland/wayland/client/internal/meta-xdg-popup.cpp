//
// Created by zhanggyb on 16-12-15.
//

#include "meta-xdg-popup.hpp"

#include <skland/wayland/client/xdg-popup.hpp>

namespace skland {
namespace wayland {
namespace client {

const struct zxdg_popup_v6_listener MetaXdgPopup::kListener = {
    OnConfigure,
    OnPopupDone
};

void MetaXdgPopup::OnConfigure(void *data,
                               struct zxdg_popup_v6 *zxdg_popup_v6,
                               int32_t x,
                               int32_t y,
                               int32_t width,
                               int32_t height) {
  XdgPopup *_this = static_cast<XdgPopup *>(data);
  if (_this->configure_)
    _this->configure_(x, y, width, height);
}

void MetaXdgPopup::OnPopupDone(void *data, struct zxdg_popup_v6 *zxdg_popup_v6) {
  XdgPopup *_this = static_cast<XdgPopup *>(data);
  if (_this->done_)
    _this->done_();
}

}
}
}