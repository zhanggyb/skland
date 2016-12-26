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

#ifndef SKLAND_GUI_MOUSE_EVENT_HPP_
#define SKLAND_GUI_MOUSE_EVENT_HPP_

#include "input-event.hpp"
#include <linux/input-event-codes.h>

namespace skland {

class AbstractSurface;

enum MouseButtonType {
  kMouseButtonLeft = BTN_LEFT,
  kMouseButtonRight = BTN_RIGHT,
  kMouseButtonMiddle = BTN_MIDDLE
};

enum MouseButtonState {
  kMouseButtonReleased = WL_POINTER_BUTTON_STATE_RELEASED,  /* 0 */
  kMouseButtonPressed = WL_POINTER_BUTTON_STATE_PRESSED /* 1 */
};

class MouseEvent : public InputEvent {

  friend class Input;

  MouseEvent() = delete;
  MouseEvent(const MouseEvent &orig) = delete;
  MouseEvent &operator=(const MouseEvent &other) = delete;

 public:

  MouseEvent(Input *input)
      : InputEvent(input),
        surface_(nullptr),
        serial_(0),
        time_(0),
        button_(0),
        state_(0),
        axis_(0) {
  }

  uint32_t serial() const {
    return serial_;
  }

  /**
   * @brief The X coordinate on wayland surface
   * @return
   */
  double surface_x() const {
    return surface_xy_.x;
  }

  /**
   * @brief The Y coordinate on wayland surface
   * @return
   */
  double surface_y() const {
    return surface_xy_.y;
  }

  double window_x() const {
    return window_xy_.x;
  }

  double window_y() const {
    return window_xy_.y;
  }

  uint32_t button() const {
    return button_;
  }

  uint32_t state() const {
    return state_;
  }

  uint32_t axis() const {
    return axis_;
  }

 private:

  ~MouseEvent() {}

  /** The surface this pointer hovers */
  AbstractSurface *surface_;

  uint32_t serial_;

  Point2D surface_xy_;

  Point2D window_xy_;

  uint32_t time_;

  uint32_t button_;
  uint32_t state_;

  uint32_t axis_;

};

}

#endif // SKLAND_GUI_MOUSE_EVENT_HPP_
