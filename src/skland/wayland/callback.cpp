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

#include <skland/wayland/callback.hpp>

namespace skland {
namespace wayland {

const struct wl_callback_listener Callback::kListener = {
    OnDone
};

void Callback::OnDone(void *data, struct wl_callback *wl_callback, uint32_t callback_data) {
  Callback *_this = static_cast<Callback *>(data);
  if (_this->done_) _this->done_(callback_data);
}

}
}
