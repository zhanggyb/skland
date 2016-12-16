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

#ifndef SKLAND_WAYLAND_CLIENT_DATA_SOURCE_HPP_
#define SKLAND_WAYLAND_CLIENT_DATA_SOURCE_HPP_

#include "data-device-manager.hpp"

namespace skland {
namespace wayland {
namespace client {

class DataSource {

  DataSource(const DataSource &) = delete;
  DataSource &operator=(const DataSource &) = delete;

 public:

  DataSource()
      : wl_data_source_(nullptr) {}

  ~DataSource() {
    if (wl_data_source_) wl_data_source_destroy(wl_data_source_);
  }

  void Setup(const DataDeviceManager &data_device_manager) {
    Destroy();

    wl_data_source_ = wl_data_device_manager_create_data_source(data_device_manager.wl_data_device_manager_);
    wl_data_source_add_listener(wl_data_source_, &kListener, this);
  }

  void Destroy() {
    if (wl_data_source_) {
      wl_data_source_destroy(wl_data_source_);
      wl_data_source_ = nullptr;
    }
  }

  DelegateRef<void(const char *)> target() {
    return target_;
  }

  DelegateRef<void(const char *, int32_t)> send() {
    return send_;
  }

  DelegateRef<void()> cancelled() {
    return cancelled_;
  }

  DelegateRef<void()> dnd_drop_performed() {
    return dnd_drop_performed_;
  }

  DelegateRef<void()> dnd_finished() {
    return dnd_finished_;
  }

  DelegateRef<void(uint32_t)> action() {
    return action_;
  }

 private:

  static const struct wl_data_source_listener kListener;

  static void OnTarget(void *data,
                       struct wl_data_source *wl_data_source,
                       const char *mime_type);

  static void OnSend(void *data,
                     struct wl_data_source *wl_data_source,
                     const char *mime_type,
                     int32_t fd);

  static void OnCancelled(void *data,
                          struct wl_data_source *wl_data_source);

  static void OnDndDropPerformed(void *data,
                                 struct wl_data_source *wl_data_source);
  static void OnDndFinished(void *data,
                            struct wl_data_source *wl_data_source);

  static void OnAction(void *data,
                       struct wl_data_source *wl_data_source,
                       uint32_t dnd_action);

  struct wl_data_source *wl_data_source_;

  Delegate<void(const char *)> target_;
  Delegate<void(const char *, int32_t)> send_;
  Delegate<void()> cancelled_;
  Delegate<void()> dnd_drop_performed_;
  Delegate<void()> dnd_finished_;
  Delegate<void(uint32_t)> action_;
};

}
}
}

#endif // SKLAND_WAYLAND_CLIENT_DATA_SOURCE_HPP_
