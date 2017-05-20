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

#ifndef SKLAND_WAYLAND_DISPLAY_HPP_
#define SKLAND_WAYLAND_DISPLAY_HPP_

#include <wayland-client.h>

namespace skland {

namespace egl {
class Display;
}

namespace wayland {

class Registry;
class Callback;

/**
 * @ingroup wayland
 * @brief A connection to the compositor.
 *
 * This class encapsulate a pointer to a native wl_display structure which
 * represents a connection to the compositor and acts as a proxy to the
 * wl_display singleton object.
 *
 * A display connection is created with Connect() (or connect_to_fd()). A
 * connection is terminated using Disconnect().
 *
 * @note A Display object is not copyable.
 */
class Display {

  friend class Registry;
  friend class Callback;
  friend class egl::Display;

  Display(const Display &) = delete;
  Display &operator=(const Display &) = delete;

 public:

  Display()
      : wl_display_(nullptr) {}

  ~Display() {
    if (wl_display_) wl_display_disconnect(wl_display_);
  }

  void Connect(const char *name = NULL) {
    Disconnect();
    wl_display_ = wl_display_connect(name);
  }

  void ConnectToFd(int fd) {
    Disconnect();
    wl_display_ = wl_display_connect_to_fd(fd);
  }

  void Disconnect() {
    if (wl_display_) {
      wl_display_disconnect(wl_display_);
      wl_display_ = nullptr;
    }
  }

  void AddListener(const struct wl_display_listener *listener, void *user_data) {
    wl_display_add_listener(wl_display_, listener, user_data);
  }

  int Dispatch() const {
    return wl_display_dispatch(wl_display_);
  }

  int DispatchPending() const {
    return wl_display_dispatch_pending(wl_display_);
  }

  int Flush() const {
    return wl_display_flush(wl_display_);
  }

  int Roundtrip() const {
    return wl_display_roundtrip(wl_display_);
  }

  int GetFd() const {
    return wl_display_get_fd(wl_display_);
  }

  int GetError() const {
    return wl_display_get_error(wl_display_);
  }

  void SetUserData(void *user_data) {
    wl_display_set_user_data(wl_display_, user_data);
  }

  void *GetUserData() const {
    return wl_display_get_user_data(wl_display_);
  }

  uint32_t GetVersion() const {
    return wl_display_get_version(wl_display_);
  }

  bool IsValid() const {
    return nullptr != wl_display_;
  }

  bool Equal(const void *object) const {
    return wl_display_ == object;
  }

 private:

  struct wl_display *wl_display_;

};

}
}

#endif // SKLAND_WAYLAND_DISPLAY_HPP_
