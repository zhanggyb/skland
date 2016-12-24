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

#include "skland/wayland/display.hpp"

namespace skland {
namespace wayland {

const struct wl_display_listener Display::kListener = {
    OnError,
    OnDeleteId
};

void Display::OnError(void *data,
                      struct wl_display *wl_display,
                      void *object_id,
                      uint32_t code,
                      const char *message) {
  Display *_this = static_cast<Display *>(data);
  if (_this->error_) {
    _this->error_.Invoke(object_id, code, message);
  }
}

void Display::OnDeleteId(void *data, struct wl_display *wl_display, uint32_t id) {
  Display *_this = static_cast<Display *>(data);
  if (_this->delete_id_) {
    _this->delete_id_.Invoke(id);
  }
}

} // namespace wayland
} // namespace skland
