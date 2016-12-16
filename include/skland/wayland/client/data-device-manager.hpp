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

#ifndef SKLAND_WAYLAND_CLIENT_DATA_DEVICE_MANAGER_HPP_
#define SKLAND_WAYLAND_CLIENT_DATA_DEVICE_MANAGER_HPP_

#include "registry.hpp"

namespace skland {
namespace wayland {
namespace client {

class DataSource;
class DataDevice;

class DataDeviceManager {

  friend class DataSource;
  friend class DataDevice;

  DataDeviceManager(const DataDeviceManager &) = delete;
  DataDeviceManager &operator=(const DataDeviceManager &) = delete;

 public:

  DataDeviceManager()
      : wl_data_device_manager_(nullptr) {}

  ~DataDeviceManager() {
    if (wl_data_device_manager_) wl_data_device_manager_destroy(wl_data_device_manager_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    wl_data_device_manager_ =
        static_cast<struct wl_data_device_manager *>(registry.Bind(id, &wl_data_device_manager_interface, version));
  }

  void Destroy() {
    if (wl_data_device_manager_) {
      wl_data_device_manager_destroy(wl_data_device_manager_);
      wl_data_device_manager_ = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_data_device_manager_set_user_data(wl_data_device_manager_, user_data);
  }

  void *GetUserData() const {
    return wl_data_device_manager_get_user_data(wl_data_device_manager_);
  }

  uint32_t GetVersion() const {
    return wl_data_device_manager_get_version(wl_data_device_manager_);
  }

 private:

  struct wl_data_device_manager *wl_data_device_manager_;

};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_DATA_DEVICE_MANAGER_HPP_
