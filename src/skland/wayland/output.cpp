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

#include <skland/wayland/output.hpp>
#include <skland/wayland/registry.hpp>

namespace skland {
namespace wayland {

const struct wl_output_listener Output::kListener = {
    OnGeometry,
    OnMode,
    OnDone,
    OnScale
};

void Output::OnGeometry(void *data,
                        struct wl_output * /* wl_output */,
                        int32_t x,
                        int32_t y,
                        int32_t physical_width,
                        int32_t physical_height,
                        int32_t subpixel,
                        const char *make,
                        const char *model,
                        int32_t transform) {
  Output *_this = static_cast<Output *>(data);
  if (_this->geometry_) {
    _this->geometry_(x, y, physical_width, physical_height, subpixel, make, model, transform);
  }
}

void Output::OnMode(void *data,
                    struct wl_output * /* wl_output */,
                    uint32_t flags,
                    int32_t width,
                    int32_t height,
                    int32_t refresh) {
  Output *_this = static_cast<Output *>(data);
  if (_this->mode_) {
    _this->mode_(flags, width, height, refresh);
  }
}

void Output::OnDone(void *data, struct wl_output * /* wl_output */) {
  Output *_this = static_cast<Output *>(data);
  if (_this->done_) {
    _this->done_();
  }
}

void Output::OnScale(void *data, struct wl_output * /* wl_output */, int32_t factor) {
  Output *_this = static_cast<Output *>(data);
  if (_this->scale_) {
    _this->scale_(factor);
  }
}

}
}
