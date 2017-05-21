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
#include "../core/size.hpp"
#include "../core/sigcxx.hpp"
#include "../core/deque.hpp"

#include <wayland-client.h>

#include <string>
#include <memory>

namespace skland {

namespace wayland {
class Output;
}

class Output : public Deque::Element {

  friend class Surface;

  Output() = delete;
  Output(const Output &) = delete;
  Output &operator=(const Output &) = delete;

 public:

  Output(uint32_t id, uint32_t version);

  virtual ~Output();

  SignalRef<Output *> destroyed() {
    return destroyed_;
  }

  const std::string &GetMake() const;

  const std::string &GetModel() const;

  uint32_t GetID() const;

  uint32_t GetVersion() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  Signal<Output *> destroyed_;

  static void OnGeometry(void *data,
                         struct wl_output *wl_output,
                         int32_t x,
                         int32_t y,
                         int32_t physical_width,
                         int32_t physical_height,
                         int32_t subpixel,
                         const char *make,
                         const char *model,
                         int32_t transform);

  static void OnMode(void *data,
                     struct wl_output *wl_output,
                     uint32_t flags,
                     int32_t width,
                     int32_t height,
                     int32_t refresh);

  static void OnDone(void *data,
                     struct wl_output *wl_output);

  static void OnScale(void *data,
                      struct wl_output *wl_output,
                      int32_t factor);

  static const struct wl_output_listener kListener;
};

}

#endif // SKLAND_GUI_OUTPUT_HPP_
