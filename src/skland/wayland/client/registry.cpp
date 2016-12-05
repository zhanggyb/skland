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

#include "skland/wayland/client/registry.hpp"

namespace skland {
namespace wayland {
namespace client {

const struct wl_registry_listener Registry::kListener = {
    OnGlobal,
    OnGlobalRemove
};

void Registry::OnGlobal(void *data,
                        struct wl_registry *registry,
                        uint32_t id,
                        const char *interface,
                        uint32_t version) {
  Registry *_this = static_cast<Registry *>(data);
  if (_this->global_) {
    _this->global_(id, interface, version);
  }
}

void Registry::OnGlobalRemove(void *data,
                              struct wl_registry *registry,
                              uint32_t name) {
  Registry *_this = static_cast<Registry *>(data);
  if (_this->global_remove_) {
    _this->global_remove_(name);
  }
}

}
}
}
