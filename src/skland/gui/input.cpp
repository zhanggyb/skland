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
#include <skland/gui/display.hpp>
#include <skland/gui/surface.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/touch-event.hpp>
#include <skland/gui/abstract-window.hpp>

#include <skland/wayland/seat.hpp>
#include <skland/wayland/pointer.hpp>
#include <skland/wayland/keyboard.hpp>
#include <skland/wayland/touch.hpp>

#include "internal/mouse-task-proxy.hpp"
#include "internal/display-proxy.hpp"
#include "internal/keymap.hpp"
#include "internal/keyboard-state.hpp"

#include <unistd.h>
#include <sys/mman.h>

#include <iostream>

namespace skland {

struct Input::Private {

  Private()
      : key_event(nullptr), mouse_event(nullptr), touch_event(nullptr) {}

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

};

Input::Input(uint32_t id, uint32_t version)
    : Object(),
      display_(nullptr) {
  data_.reset(new Private);
  data_->wl_seat.capabilities().Set(this, &Input::OnSeatCapabilities);
  data_->wl_seat.name().Set(this, &Input::OnSeatName);
  data_->wl_seat.Setup(DisplayProxy().wl_registry(), id, version);
}

Input::~Input() {
  data_.reset();

  if (display_)
    RemoveManagedObject(display_,
                        this,
                        &display_,
                        &display_->first_input_,
                        &display_->last_input_,
                        display_->inputs_count_);
  DBG_ASSERT(display_ == nullptr);
}

void Input::SetCursor(const Cursor *cursor) const {
  data_->wl_pointer.SetCursor(data_->mouse_event->serial(),
                              cursor->wl_surface(),
                              cursor->hotspot_x(), cursor->hotspot_y());
  cursor->Commit();
}

const wayland::Seat &Input::GetSeat() const {
  return data_->wl_seat;
}

void Input::OnSeatCapabilities(uint32_t capabilities) {
  if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    DBG_ASSERT(!data_->wl_keyboard.IsValid());
    data_->wl_keyboard.keymap().Set(this, &Input::OnKeyboardKeymap);
    data_->wl_keyboard.enter().Set(this, &Input::OnKeyboardEnter);
    data_->wl_keyboard.leave().Set(this, &Input::OnKeyboardLeave);
    data_->wl_keyboard.key().Set(this, &Input::OnKeyboardKey);
    data_->wl_keyboard.modifiers().Set(this, &Input::OnKeyboardModifiers);
    data_->wl_keyboard.repeat_info().Set(this, &Input::OnKeyboardRepeatInfo);
    data_->wl_keyboard.Setup(data_->wl_seat);

    DBG_ASSERT(data_->key_event == nullptr);
    data_->key_event = new KeyEvent(this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
    DBG_ASSERT(!data_->wl_pointer.IsValid());
    data_->wl_pointer.enter().Set(this, &Input::OnPointerEnter);
    data_->wl_pointer.leave().Set(this, &Input::OnPointerLeave);
    data_->wl_pointer.motion().Set(this, &Input::OnPointerMotion);
    data_->wl_pointer.button().Set(this, &Input::OnPointerButton);
    data_->wl_pointer.axis().Set(this, &Input::OnPointerAxis);
    data_->wl_pointer.frame().Set(this, &Input::OnPointerFrame);
    data_->wl_pointer.axis_source().Set(this, &Input::OnPointerAxisSource);
    data_->wl_pointer.axis_stop().Set(this, &Input::OnPointerAxisStop);
    data_->wl_pointer.axis_discrete().Set(this, &Input::OnPointerAxisDiscrete);
    data_->wl_pointer.Setup(data_->wl_seat);

    DBG_ASSERT(data_->mouse_event == nullptr);
    data_->mouse_event = new MouseEvent(this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_TOUCH) {
    DBG_ASSERT(!data_->wl_touch.IsValid());
    data_->wl_touch.down().Set(this, &Input::OnTouchDown);
    data_->wl_touch.up().Set(this, &Input::OnTouchUp);
    data_->wl_touch.motion().Set(this, &Input::OnTouchMotion);
    data_->wl_touch.frame().Set(this, &Input::OnTouchFrame);
    data_->wl_touch.cancel().Set(this, &Input::OnTouchCancel);
    data_->wl_touch.Setup(data_->wl_seat);

    DBG_ASSERT(data_->touch_event == nullptr);
    data_->touch_event = new TouchEvent(this);
  }
}

void Input::OnSeatName(const char *name) {
  data_->wl_seat.SetUserData(this);
  data_->name = name;
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
    data_->keymap.Setup(string, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
  } catch (const std::runtime_error &e) {
    close(fd);
    std::cerr << e.what() << std::endl;
    return;
  }

  munmap(string, size);
  close(fd);

  try {
    data_->keyboard_state.Setup(data_->keymap);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  // TODO: read locale
}

void Input::OnKeyboardEnter(uint32_t serial, struct wl_surface *wl_surface, struct wl_array *keys) {
  data_->key_event->serial_ = serial;
//  keyboard_event_.surface_ = static_cast<Surface*>(wl_surface_get_user_data((wl_surface)));
  // TODO: use the keys
}

void Input::OnKeyboardLeave(uint32_t serial, struct wl_surface *wl_surface) {
  data_->key_event->serial_ = serial;

}

void Input::OnKeyboardKey(uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
  data_->key_event->serial_ = serial;
  data_->key_event->time_ = serial;
  data_->key_event->key_ = key;
  data_->key_event->state_ = state;
}

void Input::OnKeyboardModifiers(uint32_t serial,
                                uint32_t mods_depressed,
                                uint32_t mods_latched,
                                uint32_t mods_locked,
                                uint32_t group) {
  data_->key_event->serial_ = serial;
  data_->key_event->mods_depressed_ = mods_depressed;
  data_->key_event->mods_latched_ = mods_latched;
  data_->key_event->mods_locked_ = mods_locked;
  data_->key_event->group_ = group;
}

void Input::OnKeyboardRepeatInfo(int32_t rate, int32_t delay) {

}

void Input::OnPointerEnter(uint32_t serial,
                           struct wl_surface *wl_surface,
                           wl_fixed_t surface_x,
                           wl_fixed_t surface_y) {
  data_->mouse_event->serial_ = serial;
  data_->mouse_event->surface_xy_.x = wl_fixed_to_double(surface_x);
  data_->mouse_event->surface_xy_.y = wl_fixed_to_double(surface_y);

  data_->mouse_event->surface_ =
      static_cast<Surface *>(wl_surface_get_user_data(wl_surface));
  data_->mouse_event->window_xy_.x =
      data_->mouse_event->surface_xy_.x - data_->mouse_event->surface_->margin().left;
  data_->mouse_event->window_xy_.y =
      data_->mouse_event->surface_xy_.y - data_->mouse_event->surface_->margin().top;

  AbstractView *view = data_->mouse_event->surface_->view();

  data_->mouse_event->response_ = InputEvent::kUnknown;
  view->OnMouseEnter(data_->mouse_event);
  if (data_->mouse_event->IsAccepted()) {
    ViewTask *task = &view->data_->mouse_task;
    DispatchMouseEnterEvent(view, task);
  }
}

void Input::OnPointerLeave(uint32_t serial, struct wl_surface *wl_surface) {
  data_->mouse_event->serial_ = serial;

  data_->mouse_event->surface_ = static_cast<Surface *>(wl_surface_get_user_data(wl_surface));
  AbstractView *view = data_->mouse_event->surface_->view();

  data_->mouse_event->response_ = InputEvent::kUnknown;
  view->OnMouseLeave(data_->mouse_event);
  if (data_->mouse_event->IsAccepted()) {
    ViewTask *task = &view->data_->mouse_task;
    ViewTask *next = nullptr;
    bool need_call = true;

    task = static_cast<ViewTask *>(task->next());
    while (task) {
      next = static_cast<ViewTask *>(task->next());
      task->Unlink();

      if (need_call) {
        data_->mouse_event->response_ = InputEvent::kUnknown;
        task->view->OnMouseLeave(data_->mouse_event);
        if (!data_->mouse_event->IsAccepted()) {
          need_call = false;
        }
      }

      task = next;
    }
  }
  data_->mouse_event->surface_ = nullptr;
}

void Input::OnPointerMotion(uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
  data_->mouse_event->time_ = time;
  data_->mouse_event->surface_xy_.x = wl_fixed_to_double(surface_x);
  data_->mouse_event->surface_xy_.y = wl_fixed_to_double(surface_y);

  if (nullptr == data_->mouse_event->surface_) return;

  data_->mouse_event->window_xy_.x =
      data_->mouse_event->surface_xy_.x - data_->mouse_event->surface_->margin().left;
  data_->mouse_event->window_xy_.y =
      data_->mouse_event->surface_xy_.y - data_->mouse_event->surface_->margin().top;

  AbstractView *view = data_->mouse_event->surface_->view();

  data_->mouse_event->response_ = InputEvent::kUnknown;
  view->OnMouseMove(data_->mouse_event);

  // The following code check if the mouse enters sub views in this surface:

  ViewTask *task = &view->data_->mouse_task;
  ViewTask *tail = task;
  while (tail->next()) {
    tail = static_cast<ViewTask *>(tail->next());
  }

  ViewTask *previous = nullptr;
  while (tail->previous()) {
    if (tail->view->Contain((int) data_->mouse_event->window_xy_.x, (int) data_->mouse_event->window_xy_.y)) {
      break;
    }
    previous = static_cast<ViewTask *>(tail->previous());

    tail->Unlink();
    data_->mouse_event->response_ = InputEvent::kUnknown;
    tail->view->OnMouseLeave(data_->mouse_event);
    // TODO: if need to check the 'accepted_' variable

    tail = previous;
  }

  // and recheck subviews:
  DispatchMouseEnterEvent(tail->view, tail);

  // Now dispatch mouse move event:
  task = static_cast<ViewTask *>(view->data_->mouse_motion_task.next());
  data_->mouse_event->response_ = InputEvent::kUnknown;
  while (task) {
    task->view->OnMouseMove(data_->mouse_event);
    if (!data_->mouse_event->IsAccepted()) break;
    task = static_cast<ViewTask *>(task->next());
  }
}

void Input::OnPointerButton(uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
  data_->mouse_event->response_ = InputEvent::kUnknown;
  data_->mouse_event->serial_ = serial;
  data_->mouse_event->time_ = time;
  data_->mouse_event->button_ = button;
  data_->mouse_event->state_ = state;

  if (nullptr == data_->mouse_event->surface_) return;

  AbstractView *view = data_->mouse_event->surface_->view();
  view->OnMouseButton(data_->mouse_event);
  if (!data_->mouse_event->IsAccepted()) return;

  MouseTaskProxy proxy(view);
  while (proxy.GetNextTask()) {
    proxy.GetNextTask()->view->OnMouseButton(data_->mouse_event);
    if (!data_->mouse_event->IsAccepted()) {
      break;
    }
    ++proxy;
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

void Input::DispatchMouseEnterEvent(AbstractView *parent, ViewTask *task) {
  for (AbstractView *view = parent->first_child(); view; view = view->next()) {
    if (view->Contain((int) data_->mouse_event->window_xy_.x, (int) data_->mouse_event->window_xy_.y)) {
      data_->mouse_event->response_ = InputEvent::kUnknown;
      view->OnMouseEnter(data_->mouse_event);
      if (data_->mouse_event->IsAccepted()) {
        task->PushBack(&view->data_->mouse_task);
        task = static_cast<ViewTask *>(task->next());
        DispatchMouseEnterEvent(view, task);
      }
      break;
    }
  }
}

}
