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

#include <skland/gui/callback.hpp>

#include <skland/wayland/callback.hpp>
#include <skland/gui/surface.hpp>

#include "internal/display_private.hpp"

namespace skland {

struct Callback::Private {

  Private() {}
  ~Private() {}

  wayland::Callback wl_callback;

  static void OnDone(void *data,
                     struct wl_callback *wl_callback,
                     uint32_t callback_data);

  static const struct wl_callback_listener kListener;

};

const struct wl_callback_listener Callback::Private::kListener = {
    OnDone
};

void Callback::Private::OnDone(void *data, struct wl_callback */*wl_callback*/, uint32_t callback_data) {
  Callback *_this = static_cast<Callback *>(data);
  if (_this->done_) _this->done_(callback_data);
}

Callback::Callback() {
  p_.reset(new Private);
}

Callback::Callback(const Display &display)
    : Callback() {
  Setup(display);
}

Callback::Callback(const Surface &surface)
    : Callback() {
  Setup(surface);
}

Callback::~Callback() {

}

void Callback::Setup(const Display &display) {
  p_->wl_callback.Setup(display.p_->wl_display);
  p_->wl_callback.AddListener(&Private::kListener, this);
}

void Callback::Setup(const Surface &surface) {
  p_->wl_callback.Setup(surface.wl_surface_);
  p_->wl_callback.AddListener(&Private::kListener, this);
}

}