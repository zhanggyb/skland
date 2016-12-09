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
#include <skland/gui/abstract-view.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/touch-event.hpp>

namespace skland {

Input::Input(const wayland::client::Registry &registry, uint32_t id, uint32_t version)
    : Object(),
      display_(nullptr),
      key_event_(nullptr),
      mouse_event_(nullptr),
      touch_event_(nullptr) {
  wl_seat_.capabilities().Set(this, &Input::OnSeatCapabilities);
  wl_seat_.name().Set(this, &Input::OnSeatName);
  wl_seat_.Setup(registry, id, version);
}

Input::~Input() {

  delete touch_event_;
  delete mouse_event_;
  delete key_event_;

  wl_touch_.Destroy();
  wl_pointer_.Destroy();
  wl_keyboard_.Destroy();
  wl_seat_.Destroy();

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
  wl_pointer_.SetCursor(mouse_event_->serial(),
                        cursor->wl_surface(),
                        cursor->hotspot_x(), cursor->hotspot_y());
  cursor->Commit();
}

void Input::OnSeatCapabilities(uint32_t capabilities) {
  if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    DBG_ASSERT(!wl_keyboard_.IsValid());
    wl_keyboard_.keymap().Set(this, &Input::OnKeyboardKeymap);
    wl_keyboard_.enter().Set(this, &Input::OnKeyboardEnter);
    wl_keyboard_.leave().Set(this, &Input::OnKeyboardLeave);
    wl_keyboard_.key().Set(this, &Input::OnKeyboardKey);
    wl_keyboard_.modifiers().Set(this, &Input::OnKeyboardModifiers);
    wl_keyboard_.repeat_info().Set(this, &Input::OnKeyboardRepeatInfo);
    wl_keyboard_.Setup(wl_seat_);

    DBG_ASSERT(key_event_ == nullptr);
    key_event_ = new KeyEvent(this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
    DBG_ASSERT(!wl_pointer_.IsValid());
    wl_pointer_.enter().Set(this, &Input::OnPointerEnter);
    wl_pointer_.leave().Set(this, &Input::OnPointerLeave);
    wl_pointer_.motion().Set(this, &Input::OnPointerMotion);
    wl_pointer_.button().Set(this, &Input::OnPointerButton);
    wl_pointer_.axis().Set(this, &Input::OnPointerAxis);
    wl_pointer_.frame().Set(this, &Input::OnPointerFrame);
    wl_pointer_.axis_source().Set(this, &Input::OnPointerAxisSource);
    wl_pointer_.axis_stop().Set(this, &Input::OnPointerAxisStop);
    wl_pointer_.axis_discrete().Set(this, &Input::OnPointerAxisDiscrete);
    wl_pointer_.Setup(wl_seat_);

    DBG_ASSERT(mouse_event_ == nullptr);
    mouse_event_ = new MouseEvent(this);
  }
  if (capabilities & WL_SEAT_CAPABILITY_TOUCH) {
    DBG_ASSERT(!wl_touch_.IsValid());
    wl_touch_.down().Set(this, &Input::OnTouchDown);
    wl_touch_.up().Set(this, &Input::OnTouchUp);
    wl_touch_.motion().Set(this, &Input::OnTouchMotion);
    wl_touch_.frame().Set(this, &Input::OnTouchFrame);
    wl_touch_.cancel().Set(this, &Input::OnTouchCancel);
    wl_touch_.Setup(wl_seat_);

    DBG_ASSERT(touch_event_ == nullptr);
    touch_event_ = new TouchEvent(this);
  }
}

void Input::OnSeatName(const char *name) {
  wl_seat_.SetUserData(this);
  name_ = name;
}

void Input::OnKeyboardKeymap(uint32_t format, int32_t fd, uint32_t size) {

}

void Input::OnKeyboardEnter(uint32_t serial, struct wl_surface *wl_surface, struct wl_array *keys) {
  key_event_->serial_ = serial;
//  keyboard_event_.surface_ = static_cast<Surface*>(wl_surface_get_user_data((wl_surface)));
  // TODO: use the keys
}

void Input::OnKeyboardLeave(uint32_t serial, struct wl_surface *wl_surface) {
  key_event_->serial_ = serial;

}

void Input::OnKeyboardKey(uint32_t serial, uint32_t time, uint32_t key, uint32_t state) {
  key_event_->serial_ = serial;
  key_event_->time_ = serial;
  key_event_->key_ = key;
  key_event_->state_ = state;
}

void Input::OnKeyboardModifiers(uint32_t serial,
                                uint32_t mods_depressed,
                                uint32_t mods_latched,
                                uint32_t mods_locked,
                                uint32_t group) {
  key_event_->serial_ = serial;
  key_event_->mods_depressed_ = mods_depressed;
  key_event_->mods_latched_ = mods_latched;
  key_event_->mods_locked_ = mods_locked;
  key_event_->group_ = group;
}

void Input::OnKeyboardRepeatInfo(int32_t rate, int32_t delay) {

}

void Input::OnPointerEnter(uint32_t serial,
                           struct wl_surface *wl_surface,
                           wl_fixed_t surface_x,
                           wl_fixed_t surface_y) {
  using gui::MouseTask;

  mouse_event_->serial_ = serial;
  mouse_event_->surface_x_ = wl_fixed_to_double(surface_x);
  mouse_event_->surface_y_ = wl_fixed_to_double(surface_y);

  mouse_event_->surface_ = static_cast<Surface *>(wl_surface_get_user_data(wl_surface));
  mouse_event_->window_x_ = mouse_event_->surface_x_ - mouse_event_->surface_->margin().left;
  mouse_event_->window_y_ = mouse_event_->surface_y_ - mouse_event_->surface_->margin().top;

  AbstractView *view_on_surface = mouse_event_->surface_->view();

  mouse_event_->accepted_ = false;
  view_on_surface->OnMouseEnter(mouse_event_);
  if (mouse_event_->is_accepted()) {
    MouseTask *task = &view_on_surface->mouse_task_;
    ProcessMouseEnterOnSubviews(view_on_surface, task);
  }
}

void Input::OnPointerLeave(uint32_t serial, struct wl_surface *wl_surface) {
  using gui::MouseTask;

  mouse_event_->serial_ = serial;

  mouse_event_->surface_ = static_cast<Surface *>(wl_surface_get_user_data(wl_surface));
  AbstractView *view_on_surface = mouse_event_->surface_->view();

  mouse_event_->accepted_ = false;
  view_on_surface->OnMouseLeave(mouse_event_);
  if (mouse_event_->is_accepted()) {
    MouseTask *task = &view_on_surface->mouse_task_;
    MouseTask *next = nullptr;
    bool need_call = true;

    task = static_cast<MouseTask *>(task->next());
    while (task) {
      next = static_cast<MouseTask *>(task->next());
      task->Unlink();

      if (need_call) {
        mouse_event_->accepted_ = false;
        task->view->OnMouseLeave(mouse_event_);
        if (!mouse_event_->is_accepted()) {
          need_call = false;
        }
      }

      task = next;
    }
  }
  mouse_event_->surface_ = nullptr;
}

void Input::OnPointerMotion(uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
  using gui::MouseTask;

  mouse_event_->time_ = time;
  mouse_event_->surface_x_ = wl_fixed_to_double(surface_x);
  mouse_event_->surface_y_ = wl_fixed_to_double(surface_y);

  if (mouse_event_->surface_) {
    mouse_event_->window_x_ = mouse_event_->surface_x_ - mouse_event_->surface_->margin().left;
    mouse_event_->window_y_ = mouse_event_->surface_y_ - mouse_event_->surface_->margin().top;

    AbstractView *view_on_surface = mouse_event_->surface_->view();

    mouse_event_->accepted_ = false;
    view_on_surface->OnMouseMove(mouse_event_);

    // The following code check if the mouse enters sub views in this surface:

    MouseTask *task = &view_on_surface->mouse_task_;
    MouseTask *tail = task;
    while (tail->next()) {
      tail = static_cast<MouseTask *>(tail->next());
    }

    MouseTask *previous = nullptr;
    while (tail->previous()) {
      if (tail->view->Contain((int) mouse_event_->window_x_, (int) mouse_event_->window_y_)) {
        break;
      }
      previous = static_cast<MouseTask *>(tail->previous());

      tail->Unlink();
      mouse_event_->accepted_ = false;
      tail->view->OnMouseLeave(mouse_event_);
      // TODO: if need to check the 'accepted_' variable

      tail = previous;
    }

    // and recheck subviews:
    ProcessMouseEnterOnSubviews(tail->view, tail);

  }
}

void Input::OnPointerButton(uint32_t serial, uint32_t time, uint32_t button, uint32_t state) {
  using gui::MouseTask;

  mouse_event_->accepted_ = false;
  mouse_event_->serial_ = serial;
  mouse_event_->time_ = time;
  mouse_event_->button_ = button;
  mouse_event_->state_ = state;

  if (mouse_event_->surface_) {
    AbstractView *view_on_surface = mouse_event_->surface_->view();

    view_on_surface->OnMouseButton(mouse_event_);
    if (mouse_event_->is_accepted()) {
      MouseTask *task = &view_on_surface->mouse_task_;
      task = static_cast<MouseTask *>(task->next());
      while (task) {
        task->view->OnMouseButton(mouse_event_);
        if (!mouse_event_->is_accepted()) {
          break;
        }
        task = static_cast<MouseTask *>(task->next());
      }
    }
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

void Input::ProcessMouseEnterOnSubviews(AbstractView *parent, gui::MouseTask *task) {
  using gui::MouseTask;

  for (AbstractView *subview = parent->first_subview(); subview; subview = subview->next_view()) {
    if (subview->Contain((int) mouse_event_->window_x_, (int) mouse_event_->window_y_)) {
      mouse_event_->accepted_ = false;
      subview->OnMouseEnter(mouse_event_);
      if (mouse_event_->is_accepted()) {
        task->AddNext(&subview->mouse_task_);
        task = static_cast<MouseTask *>(task->next());
        ProcessMouseEnterOnSubviews(subview, task);
      }
      break;
    }
  }
}

}
