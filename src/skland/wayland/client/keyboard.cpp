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

#include "skland/wayland/client/keyboard.hpp"

namespace skland {
namespace wayland {
namespace client {

const struct wl_keyboard_listener Keyboard::kListener = {
    OnKeymap,
    OnEnter,
    OnLeave,
    OnKey,
    OnModifiers,
    OnRepeatInfo
};

void Keyboard::OnKeymap(void *data, struct wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size) {
  Keyboard *_this = static_cast<Keyboard *>(data);
  if (_this->keymap_) {
    _this->keymap_.Invoke(format, fd, size);
  }
}

void Keyboard::OnEnter(void *data,
                       struct wl_keyboard *wl_keyboard,
                       uint32_t serial,
                       struct wl_surface *wl_surface,
                       struct wl_array *keys) {
  Keyboard *_this = static_cast<Keyboard *>(data);
  if (_this->enter_) {
    _this->enter_.Invoke(serial, wl_surface, keys);
  }
}

void Keyboard::OnLeave(void *data, struct wl_keyboard *wl_keyboard, uint32_t serial, struct wl_surface *wl_surface) {
  Keyboard *_this = static_cast<Keyboard *>(data);
  if (_this->leave_) {
    _this->leave_.Invoke(serial, wl_surface);
  }
}

void Keyboard::OnKey(void *data,
                     struct wl_keyboard *wl_keyboard,
                     uint32_t serial,
                     uint32_t time,
                     uint32_t key,
                     uint32_t state) {
  Keyboard *_this = static_cast<Keyboard *>(data);
  if (_this->key_) {
    _this->key_.Invoke(serial, time, key, state);
  }
}

void Keyboard::OnModifiers(void *data,
                           struct wl_keyboard *wl_keyboard,
                           uint32_t serial,
                           uint32_t mods_depressed,
                           uint32_t mods_latched,
                           uint32_t mods_locked,
                           uint32_t group) {
  Keyboard *_this = static_cast<Keyboard *>(data);
  if (_this->modifiers_) {
    _this->modifiers_.Invoke(serial, mods_depressed, mods_latched, mods_locked, group);
  }
}

void Keyboard::OnRepeatInfo(void *data, struct wl_keyboard *wl_keyboard, int32_t rate, int32_t delay) {
  Keyboard *_this = static_cast<Keyboard *>(data);
  if (_this->repeat_info_) {
    _this->repeat_info_.Invoke(rate, delay);
  }
}

}
}
}