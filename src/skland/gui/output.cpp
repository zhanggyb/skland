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

namespace skland {

Output::Output(const wayland::client::Registry &registry, uint32_t id, uint32_t version)
    : Object(),
      display_(nullptr),
      current_refresh_rate_(0),
      preferred_refresh_rate_(0),
      server_output_id_(0),
      subpixel_(0),
      transform_(WL_OUTPUT_TRANSFORM_NORMAL),
      scale_(1) {
  wl_output_.geometry().Set(this, &Output::OnGeometry);
  wl_output_.mode().Set(this, &Output::OnMode);
  wl_output_.done().Set(this, &Output::OnDone);
  wl_output_.scale().Set(this, &Output::OnScale);
  wl_output_.Setup(registry, id, version);
}

Output::~Output() {
  destroyed_.Emit(this);

  wl_output_.Destroy();

  if (display_)
    RemoveManagedObject(display_,
                        this,
                        &display_,
                        &display_->first_output_,
                        &display_->last_output_,
                        display_->outputs_count_);
  DBG_ASSERT(display_ == nullptr);
}

void Output::OnGeometry(int32_t x,
                        int32_t y,
                        int32_t physical_width,
                        int32_t physical_height,
                        int32_t subpixel,
                        const char *make,
                        const char *model,
                        int32_t transform) {
  position_.x = x;
  position_.y = y;
  physical_size_.width = physical_width;
  physical_size_.height = physical_height;
  subpixel_ = subpixel;
  make_ = make;
  model_ = model;
  transform_ = transform;
}

void Output::OnMode(uint32_t flags,
                    int32_t width,
                    int32_t height,
                    int32_t refresh) {
  if (flags & WL_OUTPUT_MODE_CURRENT) {
    current_mode_size_.width = width;
    current_mode_size_.height = height;
    current_refresh_rate_ = refresh;
  } else if (flags & WL_OUTPUT_MODE_PREFERRED) {
    preferred_mode_size_.width = width;
    preferred_mode_size_.height = height;
    preferred_refresh_rate_ = refresh;
  }
}

void Output::OnDone() {
  wl_output_.SetUserData(this);
}

void Output::OnScale(int32_t factor) {
  scale_ = factor;
}

}
