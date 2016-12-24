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

#ifndef SKLAND_GUI_OUTPUT_HPP_
#define SKLAND_GUI_OUTPUT_HPP_

#include "../core/rect.hpp"
#include "../core/object.hpp"
#include "../core/size.hpp"
#include "../wayland/output.hpp"

#include <string>


namespace skland {

class Display;

class Output : public Object {

  friend class Display;

  Output() = delete;
  Output(const Output &) = delete;
  Output &operator=(const Output &) = delete;

 public:

  Output(const wayland::Registry &registry, uint32_t id, uint32_t version);

  virtual ~Output();

  SignalRef<Output *> destroyed() {
    return destroyed_;
  }

  const std::string &make() const {
    return make_;
  }

  const std::string &model() const {
    return model_;
  }

  const Size &current_mode_size() const {
    return current_mode_size_;
  }

 private:

  void OnGeometry(int32_t x,
                  int32_t y,
                  int32_t physical_width,
                  int32_t physical_height,
                  int32_t subpixel,
                  const char *make,
                  const char *model,
                  int32_t transform);

  void OnMode(uint32_t flags,
              int32_t width,
              int32_t height,
              int32_t refresh);

  void OnDone();

  void OnScale(int32_t factor);

  Display *display_;  // manager object

  wayland::Output wl_output_;

  /** position within the global compositor space */
  Point position_;

  /** physical_width width in millimeters of the output */
  Size physical_size_;

  /** The size of a mode, given in physical hardware units of the output device */
  Size current_mode_size_;
  Size preferred_mode_size_;
  int32_t current_refresh_rate_;
  int32_t preferred_refresh_rate_;

  uint32_t server_output_id_;

  int subpixel_;  /**< enum value of wl_output_subpixel */
  int transform_; /**< enum value of wl_output_transform */
  int scale_;

  /* vertical refresh rate in mHz */

  std::string make_;
  std::string model_;

  Signal<Output *> destroyed_;
  // TODO: user data
};

}

#endif // SKLAND_GUI_OUTPUT_HPP_
