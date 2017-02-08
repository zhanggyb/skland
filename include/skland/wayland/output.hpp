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

#ifndef SKLAND_WAYLAND_OUTPUT_HPP_
#define SKLAND_WAYLAND_OUTPUT_HPP_

#include "registry.hpp"

#include <wayland-client.h>
#include <skland/core/delegate.hpp>

namespace skland {
namespace wayland {

class XdgToplevel;

class Output {

  friend class XdgToplevel;

  Output(const Output &) = delete;
  Output &operator=(const Output &) = delete;

 public:

  Output()
      : wl_output_(nullptr) {}

  ~Output() {
    if (wl_output_) wl_output_destroy(wl_output_);
  }

  void Setup(const Registry &registry, uint32_t id, uint32_t version) {
    Destroy();

    wl_output_ = static_cast<struct wl_output *>(registry.Bind(id, &wl_output_interface, version));
    wl_output_add_listener(wl_output_, &kListener, this);
  }

  void Destroy() {
    if (wl_output_) {
      wl_output_destroy(wl_output_);
      wl_output_ = nullptr;
    }
  }

  /**
   * @brief Set user data
   * @param user_data
   *
   * @warning This should be called in OnDone(), see wayland protocol.
   */
  void SetUserData(void *user_data) {
    wl_output_set_user_data(wl_output_, user_data);
  }

  void *GetUserData() const {
    return wl_output_get_user_data(wl_output_);
  }

  uint32_t GetVersion() const {
    return wl_output_get_version(wl_output_);
  }

  bool IsValid() const {
    return nullptr != wl_output_;
  }

  bool Equal(const void *object) const {
    return wl_output_ == object;
  }

  DelegateRef<void(int32_t, int32_t, int32_t, int32_t, int32_t,
                   const char *, const char *, int32_t)> geometry() {
    return geometry_;
  }

  DelegateRef<void(uint32_t, int32_t, int32_t, int32_t)> mode() {
    return mode_;
  }

  DelegateRef<void()> done() {
    return done_;
  }

  DelegateRef<void(int32_t)> scale() {
    return scale_;
  }

 private:

  static void OnGeometry(void *data,
                         struct wl_output *wl_output,
                         int32_t x,
                         int32_t y,
                         int32_t physical_width,
                         int32_t physical_height,
                         int32_t subpixel,
                         const char *make,
                         const char *model,
                         int32_t transform);

  static void OnMode(void *data,
                     struct wl_output *wl_output,
                     uint32_t flags,
                     int32_t width,
                     int32_t height,
                     int32_t refresh);

  static void OnDone(void *data,
                     struct wl_output *wl_output);

  static void OnScale(void *data,
                      struct wl_output *wl_output,
                      int32_t factor);

  static const struct wl_output_listener kListener;

  struct wl_output *wl_output_;

  Delegate<void(int32_t, int32_t, int32_t, int32_t, int32_t,
                const char *, const char *, int32_t)> geometry_;

  Delegate<void(uint32_t, int32_t, int32_t, int32_t)> mode_;

  Delegate<void()> done_;

  Delegate<void(int32_t)> scale_;

};

}
}

#endif // SKLAND_WAYLAND_OUTPUT_HPP_
