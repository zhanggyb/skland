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

#ifndef SKLAND_WAYLAND_CURSOR_THEME_HPP_
#define SKLAND_WAYLAND_CURSOR_THEME_HPP_

#include <wayland-client.h>

namespace skland {
namespace wayland {

class CursorTheme {

  CursorTheme(const CursorTheme &) = delete;
  CursorTheme &operator=(const CursorTheme &) = delete;

 public:

  CursorTheme()
      : wl_cursor_theme_(nullptr) {}

  ~CursorTheme() {
    if (wl_cursor_theme_) wl_cursor_theme_destroy(wl_cursor_theme_);
  }

  void Load(const char *name, int size, struct wl_shm *shm) {
    Destroy();

    wl_cursor_theme_ = wl_cursor_theme_load(name, size, shm);
  }

  void Destroy() {
    if (wl_cursor_theme_) {
      wl_cursor_theme_destroy(wl_cursor_theme_);
      wl_cursor_theme_ = nullptr;
    }
  }

  struct wl_cursor *GetCursor(const char *name) const {
    return wl_cursor_theme_get_cursor(wl_cursor_theme_, name);
  }

  bool IsValid() const {
    return nullptr != wl_cursor_theme_;
  }

 private:

  struct wl_cursor_theme *wl_cursor_theme_;

};

}
}

#endif // SKLAND_WAYLAND_CURSOR_THEME_HPP_
