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

#include "skland/wayland/touch.hpp"

namespace skland {
namespace wayland {

const struct wl_touch_listener Touch::kListener = {
    OnDown,
    OnUp,
    OnMotion,
    OnFrame,
    OnCancel
};

void Touch::OnDown(void *data,
                   struct wl_touch *wl_touch,
                   uint32_t serial,
                   uint32_t time,
                   struct wl_surface *surface,
                   int32_t id,
                   wl_fixed_t x,
                   wl_fixed_t y) {
  Touch *_this = static_cast<Touch *>(data);
  if (_this->down_) {
    _this->down_.Invoke(serial, time, surface, id, x, y);
  }
}

void Touch::OnUp(void *data, struct wl_touch *wl_touch, uint32_t serial, uint32_t time, int32_t id) {
  Touch *_this = static_cast<Touch *>(data);
  if (_this->up_) {
    _this->up_.Invoke(serial, time, id);
  }
}

void Touch::OnMotion(void *data, struct wl_touch *wl_touch, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {
  Touch *_this = static_cast<Touch *>(data);
  if (_this->motion_) {
    _this->motion_.Invoke(time, id, x, y);
  }
}

void Touch::OnFrame(void *data, struct wl_touch *wl_touch) {
  Touch *_this = static_cast<Touch *>(data);
  if (_this->frame_) {
    _this->frame_.Invoke();
  }
}

void Touch::OnCancel(void *data, struct wl_touch *wl_touch) {
  Touch *_this = static_cast<Touch *>(data);
  if (_this->cancel_) {
    _this->cancel_.Invoke();
  }
}

}
}
