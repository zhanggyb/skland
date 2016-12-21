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

#include "skland/wayland/client/pointer.hpp"
#include "skland/wayland/client/seat.hpp"

namespace skland {
namespace wayland {
namespace client {

const struct wl_pointer_listener Pointer::kListener = {
    OnEnter,
    OnLeave,
    OnMotion,
    OnButton,
    OnAxis,
    OnFrame,
    OnAxisSource,
    OnAxisStop,
    OnAxisDiscrete
};

void Pointer::OnEnter(void *data,
                      struct wl_pointer *wl_pointer,
                      uint32_t serial,
                      struct wl_surface *wl_surface,
                      wl_fixed_t surface_x,
                      wl_fixed_t surface_y) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->enter_) {
    _this->enter_(serial, wl_surface, surface_x, surface_y);
  }
}

void Pointer::OnLeave(void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *wl_surface) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->leave_) {
    _this->leave_(serial, wl_surface);
  }
}

void Pointer::OnMotion(void *data,
                       struct wl_pointer *wl_pointer,
                       uint32_t time,
                       wl_fixed_t surface_x,
                       wl_fixed_t surface_y) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->motion_) {
    _this->motion_(time, surface_x, surface_y);
  }
}

void Pointer::OnButton(void *data,
                       struct wl_pointer *wl_pointer,
                       uint32_t serial,
                       uint32_t time,
                       uint32_t button,
                       uint32_t state) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->button_) {
    _this->button_(serial, time, button, state);
  }
}

void Pointer::OnAxis(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->axis_) {
    _this->axis_(time, axis, value);
  }
}

void Pointer::OnFrame(void *data, struct wl_pointer *wl_pointer) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->frame_) {
    _this->frame_();
  }
}

void Pointer::OnAxisSource(void *data, struct wl_pointer *wl_pointer, uint32_t axis_source) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->axis_source_) {
    _this->axis_source_(axis_source);
  }
}

void Pointer::OnAxisStop(void *data, struct wl_pointer *wl_pointer, uint32_t time, uint32_t axis) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->axis_stop_) {
    _this->axis_stop_(time, axis);
  }
}

void Pointer::OnAxisDiscrete(void *data, struct wl_pointer *wl_pointer, uint32_t axis, int32_t discrete) {
  Pointer *_this = static_cast<Pointer *>(data);
  if (_this->axis_discrete_) {
    _this->axis_discrete_(axis, discrete);
  }
}

}
}
}
