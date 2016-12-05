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

#ifndef SKLAND_GUI_CURSOR_HPP_
#define SKLAND_GUI_CURSOR_HPP_

#include <wayland-cursor.h>

#include <skland/wayland/client/surface.hpp>

namespace skland {

class Cursor {

  Cursor(const Cursor &) = delete;
  Cursor &operator=(const Cursor &) = delete;

 public:

  static Cursor *Create(struct wl_cursor *wl_cursor);

  ~Cursor();

  void Commit() const {
    wl_surface_.Commit();
  }

  const wayland::client::Surface &wl_surface() const {
    return wl_surface_;
  }

  int32_t hotspot_x() const {
    return wl_cursor_->images[wl_cursor_->image_count - 1]->hotspot_x;
  }

  int32_t hotspot_y() const {
    return wl_cursor_->images[wl_cursor_->image_count - 1]->hotspot_y;
  }

 private:

  Cursor() : wl_cursor_(nullptr) {}

  wayland::client::Surface wl_surface_;

  struct wl_cursor *wl_cursor_;

};

}

#endif // SKLAND_GUI_CURSOR_HPP_
