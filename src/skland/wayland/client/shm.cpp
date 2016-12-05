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

#include "skland/wayland/client/shm.hpp"
#include "skland/wayland/client/registry.hpp"

namespace skland {
namespace wayland {
namespace client {

const struct wl_shm_listener Shm::kListener = {
    OnFormat
};

void Shm::OnFormat(void *data, struct wl_shm *shm, uint32_t format) {
  Shm *_this = static_cast<Shm *>(data);
  if (_this->format_) {
    _this->format_.Invoke(format);
  }
}

}
}
}
