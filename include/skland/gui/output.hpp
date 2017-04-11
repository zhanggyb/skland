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

#include <string>
#include <memory>

namespace skland {

namespace wayland {
class Output;
}

class Output {

  template<typename T> friend
  class Deque;

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

  const wayland::Output &GetOutput() const;

  uint32_t GetID() const;

  uint32_t GetVersion() const;

  Output *previous() const { return previous_; }

  Output *next() const { return next_; }

 private:

  struct Private;

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

  Output *previous_;
  Output *next_;
  Deque<Output> *deque_;

  std::unique_ptr<Private> p_;

  Signal<Output *> destroyed_;
  // TODO: user data
};

}

#endif // SKLAND_GUI_OUTPUT_HPP_
