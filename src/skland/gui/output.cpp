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

#include "internal/output_private.hpp"

#include <skland/gui/display.hpp>

#include "internal/display_native.hpp"

namespace skland {

const struct wl_output_listener Output::kListener = {
    OnGeometry,
    OnMode,
    OnDone,
    OnScale
};

void Output::OnGeometry(void *data,
                        struct wl_output * /* wl_output */,
                        int32_t x,
                        int32_t y,
                        int32_t physical_width,
                        int32_t physical_height,
                        int32_t subpixel,
                        const char *make,
                        const char *model,
                        int32_t transform) {
  Output *_this = static_cast<Output *>(data);

  _this->p_->position_.x = x;
  _this->p_->position_.y = y;
  _this->p_->physical_size_.width = physical_width;
  _this->p_->physical_size_.height = physical_height;
  _this->p_->subpixel = subpixel;
  _this->p_->make_ = make;
  _this->p_->model_ = model;
  _this->p_->transform = transform;
}

void Output::OnMode(void *data,
                    struct wl_output * /* wl_output */,
                    uint32_t flags,
                    int32_t width,
                    int32_t height,
                    int32_t refresh) {
  Output *_this = static_cast<Output *>(data);

  if (flags & WL_OUTPUT_MODE_CURRENT) {
    _this->p_->current_mode_size_.width = width;
    _this->p_->current_mode_size_.height = height;
    _this->p_->current_refresh_rate = refresh;
  } else if (flags & WL_OUTPUT_MODE_PREFERRED) {
    _this->p_->preferred_mode_size_.width = width;
    _this->p_->preferred_mode_size_.height = height;
    _this->p_->preferred_refresh_rate = refresh;
  }
}

void Output::OnDone(void *data, struct wl_output * /* wl_output */) {
//  Output *_this = static_cast<Output *>(data);
}

void Output::OnScale(void *data, struct wl_output * /* wl_output */, int32_t factor) {
  Output *_this = static_cast<Output *>(data);
  _this->p_->scale = factor;
}

Output::Output(uint32_t id, uint32_t version)
    : Deque::Element() {
  p_.reset(new Private);
  p_->id = id;
  p_->version = version;

  p_->wl_output =
      static_cast<struct wl_output *>(wl_registry_bind(Display::Native().wl_registry(),
                                                       id,
                                                       &wl_output_interface,
                                                       version));
  wl_output_add_listener(p_->wl_output, &kListener, this);
}

Output::~Output() {
  destroyed_.Emit(this);
}

const std::string &Output::GetMake() const {
  return p_->make_;
}

const std::string &Output::GetModel() const {
  return p_->model_;
}

uint32_t Output::GetID() const {
  return p_->id;
}

uint32_t Output::GetVersion() const {
  return p_->version;
}

}
