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

#include <skland/wayland/client/data-device.hpp>

namespace skland {
namespace wayland {
namespace client {

const struct wl_data_device_listener DataDevice::kListener = {
    OnDataOffer,
    OnEnter,
    OnLeave,
    OnMotion,
    OnDrop,
    OnSelection
};

void DataDevice::OnDataOffer(void *data, struct wl_data_device *wl_data_device, struct wl_data_offer *id) {

}

void DataDevice::OnEnter(void *data,
                         struct wl_data_device *wl_data_device,
                         uint32_t serial,
                         struct wl_surface *surface,
                         wl_fixed_t x,
                         wl_fixed_t y,
                         struct wl_data_offer *id) {

}

void DataDevice::OnLeave(void *data, struct wl_data_device *wl_data_device) {

}

void DataDevice::OnMotion(void *data,
                          struct wl_data_device *wl_data_device,
                          uint32_t time,
                          wl_fixed_t x,
                          wl_fixed_t y) {

}

void DataDevice::OnDrop(void *data, struct wl_data_device *wl_data_device) {

}

void DataDevice::OnSelection(void *data, struct wl_data_device *wl_data_device, struct wl_data_offer *id) {

}

}
}
}
