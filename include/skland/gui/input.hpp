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

#ifndef SKLAND_GUI_INPUT_HPP_
#define SKLAND_GUI_INPUT_HPP_

#include "../core/object.hpp"

#include "../wayland/client/seat.hpp"
#include "../wayland/client/pointer.hpp"
#include "../wayland/client/keyboard.hpp"
#include "../wayland/client/touch.hpp"

#include "skland/gui/internal/mouse-task.hpp"

namespace skland {

class Display;
class Cursor;
class KeyEvent;
class MouseEvent;
class TouchEvent;

class Input : public Object {

  friend class Display;

  Input() = delete;
  Input(const Input &) = delete;
  Input &operator=(const Input &) = delete;

 public:

  Input(const wayland::client::Registry &registry, uint32_t id, uint32_t version);

  virtual ~Input();

  void SetCursor(const Cursor *cursor) const;

  const wayland::client::Seat &wl_seat() const {
    return wl_seat_;
  }

 private:

  void OnSeatCapabilities(uint32_t capabilities);

  void OnSeatName(const char *name);

  // keyboard:

  void OnKeyboardKeymap(uint32_t format,
                        int32_t fd,
                        uint32_t size);

  void OnKeyboardEnter(uint32_t serial,
                       struct wl_surface *wl_surface,
                       struct wl_array *keys);

  void OnKeyboardLeave(uint32_t serial,
                       struct wl_surface *wl_surface);

  void OnKeyboardKey(uint32_t serial,
                     uint32_t time,
                     uint32_t key,
                     uint32_t state);

  void OnKeyboardModifiers(uint32_t serial,
                           uint32_t mods_depressed,
                           uint32_t mods_latched,
                           uint32_t mods_locked,
                           uint32_t group);

  void OnKeyboardRepeatInfo(int32_t rate,
                            int32_t delay);

  // pointer:

  void OnPointerEnter(uint32_t serial,
                      struct wl_surface *wl_surface,
                      wl_fixed_t surface_x,
                      wl_fixed_t surface_y);

  void OnPointerLeave(uint32_t serial,
                      struct wl_surface *wl_surface);

  void OnPointerMotion(uint32_t time,
                       wl_fixed_t surface_x,
                       wl_fixed_t surface_y);

  void OnPointerButton(uint32_t serial,
                       uint32_t time,
                       uint32_t button,
                       uint32_t state);

  void OnPointerAxis(uint32_t time,
                     uint32_t axis,
                     wl_fixed_t value);

  void OnPointerFrame();

  void OnPointerAxisSource(uint32_t axis_source);

  void OnPointerAxisStop(uint32_t time,
                         uint32_t axis);

  void OnPointerAxisDiscrete(uint32_t axis,
                             int32_t discrete);

  // touch:

  void OnTouchDown(uint32_t serial,
                   uint32_t time,
                   struct wl_surface *surface,
                   int32_t id,
                   wl_fixed_t x,
                   wl_fixed_t y);

  void OnTouchUp(uint32_t serial,
                 uint32_t time,
                 int32_t id);

  void OnTouchMotion(uint32_t time,
                     int32_t id,
                     wl_fixed_t x,
                     wl_fixed_t y);

  void OnTouchFrame();

  void OnTouchCancel();

  void ProcessMouseEnterOnSubviews(AbstractView *parent, gui::MouseTask *task);

  Display *display_;

  wayland::client::Seat wl_seat_;
  wayland::client::Keyboard wl_keyboard_;
  wayland::client::Pointer wl_pointer_;
  wayland::client::Touch wl_touch_;

  KeyEvent *key_event_;
  MouseEvent *mouse_event_;
  TouchEvent *touch_event_;

  std::string name_;

};

}

#endif // SKLAND_GUI_INPUT_HPP_
