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

#ifndef SKLAND_WAYLAND_DATA_DEVICE_HPP_
#define SKLAND_WAYLAND_DATA_DEVICE_HPP_

#include "data-device-manager.hpp"
#include "seat.hpp"

namespace skland {
namespace wayland {

class DataDevice {

 public:

  DataDevice()
      : wl_data_device_(nullptr) {}

  ~DataDevice() {
    if (wl_data_device_) wl_data_device_destroy(wl_data_device_);
  }

  void Setup(const DataDeviceManager &data_device_manager, const Seat &seat) {
    Destroy();
    wl_data_device_ =
        wl_data_device_manager_get_data_device(data_device_manager.wl_data_device_manager_, seat.wl_seat_);
  }

  void Destroy() {
    if (wl_data_device_) {
      wl_data_device_destroy(wl_data_device_);
      wl_data_device_ = nullptr;
    }
  }

  void AddListener(const struct wl_data_device_listener* listener, void *user_data) {
    wl_data_device_add_listener(wl_data_device_, listener, user_data);
  }

 private:

  struct wl_data_device *wl_data_device_;

};

}
}

#endif // SKLAND_WAYLAND_DATA_DEVICE_HPP_
