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

#include <skland/gui/input.hpp>

#include <skland/gui/surface.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/touch-event.hpp>

#include <skland/wayland/seat.hpp>
#include <skland/wayland/pointer.hpp>
#include <skland/wayland/keyboard.hpp>
#include <skland/wayland/touch.hpp>

#include "internal/display-registry.hpp"
#include "internal/keymap.hpp"
#include "internal/keyboard-state.hpp"
#include "internal/mouse-event-private.hpp"

#include <unistd.h>
#include <sys/mman.h>

#include <iostream>
#include <skland/core/assert.hpp>

namespace skland {

struct Input::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : key_event(nullptr),
        mouse_event(nullptr),
        touch_event(nullptr),
        id(0), version(0) {}

  ~Private() {
    keyboard_state.Destroy();
    keymap.Destroy();

    delete touch_event;
    delete mouse_event;
    delete key_event;

    wl_touch.Destroy();
    wl_pointer.Destroy();
    wl_keyboard.Destroy();
    wl_seat.Destroy();
  }

  wayland::Seat wl_seat;
  wayland::Keyboard wl_keyboard;
  wayland::Pointer wl_pointer;
  wayland::Touch wl_touch;

  KeyEvent *key_event;
  MouseEvent *mouse_event;
  TouchEvent *touch_event;

  std::string name;

  Keymap keymap;
  KeyboardState keyboard_state;

  uint32_t id;
  uint32_t version;
};

Input::Input(uint32_t id, uint32_t version)
    : Deque::Element() {
  p_.reset(new Private);
  p_->id = id;
  p_->version = version;

  p_->wl_seat.capabilities().Set(this, &Input::OnSeatCapabilities);
  p_->wl_seat.name().Set(this, &Input::OnSeatName);
  p_->wl_seat.Setup(Display::Registry().wl_registry(), p_->id, p_->version);
}

Input::~Input() {
  p_.reset();
}

void Input::SetCursor(const Cursor *cursor) const {
  p_->wl_pointer.SetCursor(p_->mouse_event->GetSerial(),
                           cursor->wl_surface(),
                           cursor->hotspot_x(), cursor->hotspot_y());
  cursor->Commit();
}

const wayland::Seat &Input::GetSeat() const {
  return p_->wl_seat;
}

uint32_t Input::GetID() const {
  return p_->id;
}

uint32_t Input::GetVersion() const {
  return p_->version;
}

void Input::OnSeatCapabilities(uint32_t capabilities) {
  if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    _ASSERT(!p_->wl_keyboard.IsValid());
    p_->wl_keyboard.keymap().Set(this, &Input::OnKeyboardKeymap);
    p_->wl_keyboard.enter().Set(this, &Input::OnKeyboardEnter);
    p_->wl_keyboard.leave().Set(this, &Input::OnKeyboardLeave);
    p_->wl_keyboard.key().Set(this, &Input::OnKeyboardKey);
    p_->wl_keyboard.modifiers().Set(this, &Input::OnKeyboardModifiers);
    p_->wl_keyboard.repeat_info().Set(this, &Input::OnKeyboardRepeatInfo);
    p_->wl_keyboard.Setup(p_->wl_seat);

    _ASSERT(p_->key_event == nullptr);
    p_->key_event = new KeyEvent(this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
    _ASSERT(!p_->wl_pointer.IsValid());
    p_->wl_pointer.enter().Set(this, &Input::OnPointerEnter);
    p_->wl_pointer.leave().Set(this, &Input::OnPointerLeave);
    p_->wl_pointer.motion().Set(this, &Input::OnPointerMotion);
    p_->wl_pointer.button().Set(this, &Input::OnPointerButton);
    p_->wl_pointer.axis().Set(this, &Input::OnPointerAxis);
    p_->wl_pointer.frame().Set(this, &Input::OnPointerFrame);
    p_->wl_pointer.axis_source().Set(this, &Input::OnPointerAxisSource);
    p_->wl_pointer.axis_stop().Set(this, &Input::OnPointerAxisStop);
    p_->wl_pointer.axis_discrete().Set(this, &Input::OnPointerAxisDiscrete);
    p_->wl_pointer.Setup(p_->wl_seat);

    _ASSERT(p_->mouse_event == nullptr);
    p_->mouse_event = new MouseEvent(this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_TOUCH) {
    _ASSERT(!p_->wl_touch.IsValid());
    p_->wl_touch.down().Set(this, &Input::OnTouchDown);
    p_->wl_touch.up().Set(this, &Input::OnTouchUp);
    p_->wl_touch.motion().Set(this, &Input::OnTouchMotion);
    p_->wl_touch.frame().Set(this, &Input::OnTouchFrame);
    p_->wl_touch.cancel().Set(this, &Input::OnTouchCancel);
    p_->wl_touch.Setup(p_->wl_seat);

    _ASSERT(p_->touch_event == nullptr);
    p_->touch_event = new TouchEvent(this);
  }
}

void Input::OnSeatName(const char *name) {
  p_->wl_seat.SetUserData(this);
  p_->name = name;
}

void Input::OnKeyboardKeymap(uint32_t format, int32_t fd, uint32_t size) {
  if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
    close(fd);
    return;
  }

  char *string = (char *) mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
  if (string == MAP_FAILED) {
    close(fd);
    return;
  }

  try {
    p_->keymap.Setup(string, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
  } catch (const std::runtime_error &e) {
    close(fd);
    std::cerr << e.what() << std::endl;
    return;
  }

  munmap(string, size);
  close(fd);

  try {
    p_->keyboard_state.Setup(p_->keymap);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  // TODO: read locale
}

void Input::OnKeyboardEnter(uint32_t serial, struct wl_surface *wl_surface, struct wl_array *keys) {
  p_->key_event->serial_ = serial;
//  keyboard_event_.surface_ = static_cast<Surface*>(wl_surface_get_user_data((wl_surface)));
  // TODO: use the keys
//  void *p = nullptr;
//  int count = 0;
//  wl_array_for_each(p, keys) {
//    count++;
//  }
}

void Input::OnKeyboardLeave(uint32_t serial, struct wl_surface *wl_surface) {
  p_->key_event->serial_ = serial;

}

void Input::OnKeyboardKey(uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
  p_->key_event->serial_ = serial;
  p_->key_event->time_ = serial;
  p_->key_event->key_ = key;
  p_->key_event->state_ = state;

  if (state == WL_KEYBOARD_KEY_STATE_PRESSED) {
    // OnKeyDown;
  } else if (state == WL_KEYBOARD_KEY_STATE_RELEASED) {
    // OnKeyUp;
  }
}

void Input::OnKeyboardModifiers(uint32_t serial,
                                uint32_t mods_depressed,
                                uint32_t mods_latched,
                                uint32_t mods_locked,
                                uint32_t group) {
  p_->key_event->serial_ = serial;
  p_->key_event->mods_depressed_ = mods_depressed;
  p_->key_event->mods_latched_ = mods_latched;
  p_->key_event->mods_locked_ = mods_locked;
  p_->key_event->group_ = group;
}

void Input::OnKeyboardRepeatInfo(int32_t rate, int32_t delay) {

}

void Input::OnPointerEnter(uint32_t serial,
                           struct wl_surface *wl_surface,
                           wl_fixed_t surface_x,
                           wl_fixed_t surface_y) {
  p_->mouse_event->p_->serial = serial;
  p_->mouse_event->p_->surface_xy.x = wl_fixed_to_double(surface_x);
  p_->mouse_event->p_->surface_xy.y = wl_fixed_to_double(surface_y);

  p_->mouse_event->p_->surface =
      static_cast<Surface *>(wl_surface_get_user_data(wl_surface));

  p_->mouse_event->response_ = InputEvent::kUnknown;
  p_->mouse_event->p_->surface->event_handler()->OnMouseEnter(p_->mouse_event);
}

void Input::OnPointerLeave(uint32_t serial, struct wl_surface *wl_surface) {
  p_->mouse_event->p_->serial = serial;

  p_->mouse_event->p_->surface = static_cast<Surface *>(wl_surface_get_user_data(wl_surface));

  p_->mouse_event->response_ = InputEvent::kUnknown;
  p_->mouse_event->p_->surface->event_handler()->OnMouseLeave();
}

void Input::OnPointerMotion(uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
  p_->mouse_event->p_->time = time;
  p_->mouse_event->p_->surface_xy.x = wl_fixed_to_double(surface_x);
  p_->mouse_event->p_->surface_xy.y = wl_fixed_to_double(surface_y);

  if (nullptr == p_->mouse_event->p_->surface) return;

  p_->mouse_event->response_ = InputEvent::kUnknown;
  p_->mouse_event->p_->surface->event_handler()->OnMouseMove(p_->mouse_event);
}

void Input::OnPointerButton(uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
  p_->mouse_event->response_ = InputEvent::kUnknown;
  p_->mouse_event->p_->serial = serial;
  p_->mouse_event->p_->time = time;
  p_->mouse_event->p_->button = button;
  p_->mouse_event->p_->state = state;

  if (nullptr == p_->mouse_event->p_->surface) return;

  p_->mouse_event->response_ = InputEvent::kUnknown;

  if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
    p_->mouse_event->p_->surface->event_handler()->OnMouseDown(p_->mouse_event);
  } else if (state == WL_POINTER_BUTTON_STATE_RELEASED) {
    p_->mouse_event->p_->surface->event_handler()->OnMouseUp(p_->mouse_event);
  }
}

void Input::OnPointerAxis(uint32_t time, uint32_t axis, wl_fixed_t value) {

}

void Input::OnPointerFrame() {

}

void Input::OnPointerAxisSource(uint32_t axis_source) {

}

void Input::OnPointerAxisStop(uint32_t time, uint32_t axis) {

}

void Input::OnPointerAxisDiscrete(uint32_t axis, int32_t discrete) {

}

void Input::OnTouchDown(uint32_t serial,
                        uint32_t time,
                        struct wl_surface *surface,
                        int32_t id,
                        wl_fixed_t x,
                        wl_fixed_t y) {

}

void Input::OnTouchUp(uint32_t serial, uint32_t time, int32_t id) {

}

void Input::OnTouchMotion(uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y) {

}

void Input::OnTouchFrame() {

}

void Input::OnTouchCancel() {

}

}
