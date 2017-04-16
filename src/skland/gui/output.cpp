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

#include <skland/gui/output.hpp>
#include <skland/gui/display.hpp>

#include <skland/wayland/output.hpp>

#include "internal/display-registry.hpp"

namespace skland {

struct Output::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : current_refresh_rate(0),
        preferred_refresh_rate(0),
        subpixel(0),
        transform(WL_OUTPUT_TRANSFORM_NORMAL),
        scale(1),
        id(0),
        version(0) {}

  ~Private() {}

  wayland::Output wl_output;

  /** position within the global compositor space */
  Point position_;

  /** physical_width width in millimeters of the output */
  Size physical_size_;

  /** The size of a mode, given in physical hardware units of the output device */
  Size current_mode_size_;
  Size preferred_mode_size_;
  int32_t current_refresh_rate;
  int32_t preferred_refresh_rate;

  int subpixel;  /**< enum value of wl_output_subpixel */
  int transform; /**< enum value of wl_output_transform */
  int scale;

  /* vertical refresh rate in mHz */

  std::string make_;
  std::string model_;

  uint32_t id;
  uint32_t version;
};

Output::Output(uint32_t id, uint32_t version)
    : Deque::Element() {
  p_.reset(new Private);
  p_->id = id;
  p_->version = version;

  p_->wl_output.geometry().Set(this, &Output::OnGeometry);
  p_->wl_output.mode().Set(this, &Output::OnMode);
  p_->wl_output.done().Set(this, &Output::OnDone);
  p_->wl_output.scale().Set(this, &Output::OnScale);

  p_->wl_output.Setup(Display::Registry().wl_registry(), p_->id, p_->version);
}

Output::~Output() {
  destroyed_.Emit(this);
  p_->wl_output.Destroy();
}

const std::string &Output::GetMake() const {
  return p_->make_;
}

const std::string &Output::GetModel() const {
  return p_->model_;
}

const wayland::Output &Output::GetOutput() const {
  return p_->wl_output;
}

uint32_t Output::GetID() const {
  return p_->id;
}

uint32_t Output::GetVersion() const {
  return p_->version;
}

void Output::OnGeometry(int32_t x,
                        int32_t y,
                        int32_t physical_width,
                        int32_t physical_height,
                        int32_t subpixel,
                        const char *make,
                        const char *model,
                        int32_t transform) {
  p_->position_.x = x;
  p_->position_.y = y;
  p_->physical_size_.width = physical_width;
  p_->physical_size_.height = physical_height;
  p_->subpixel = subpixel;
  p_->make_ = make;
  p_->model_ = model;
  p_->transform = transform;
}

void Output::OnMode(uint32_t flags,
                    int32_t width,
                    int32_t height,
                    int32_t refresh) {
  if (flags & WL_OUTPUT_MODE_CURRENT) {
    p_->current_mode_size_.width = width;
    p_->current_mode_size_.height = height;
    p_->current_refresh_rate = refresh;
  } else if (flags & WL_OUTPUT_MODE_PREFERRED) {
    p_->preferred_mode_size_.width = width;
    p_->preferred_mode_size_.height = height;
    p_->preferred_refresh_rate = refresh;
  }
}

void Output::OnDone() {
  p_->wl_output.SetUserData(this);
}

void Output::OnScale(int32_t factor) {
  p_->scale = factor;
}

}
