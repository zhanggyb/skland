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

#include <skland/gui/abstract-button.hpp>

#include <skland/core/numeric.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

namespace skland {

AbstractButton::AbstractButton()
    : AbstractWidget(),
      flags_(0x1) {

}

AbstractButton::~AbstractButton() {

}

void AbstractButton::OnMouseEnter(MouseEvent *event) {
  set_bit<uint32_t>(flags_, kFlagIndexHovered);
  if (flags_ && kFlagIndexSensitive) {
    Damage(geometry());
    Show();
  }
  event->Accept();
}

void AbstractButton::OnMouseLeave(MouseEvent *event) {
  clear_bit<uint32_t>(flags_, kFlagIndexHovered);
  clear_bit<uint32_t>(flags_, kFlagIndexClick);
  if (flags_ && kFlagIndexSensitive) {
    Damage(geometry());
    Show();
  }
  event->Accept();
}

void AbstractButton::OnMouseButton(MouseEvent *event) {
  if (event->state() == kMouseButtonPressed) {
    set_bit<uint32_t>(flags_, kFlagIndexPressed);
    Damage(geometry());
    Show();
  } else {
    if (flags_ & kFlagIndexPressed) {
      set_bit<uint32_t>(flags_, kFlagIndexClick);
    }
    clear_bit<uint32_t>(flags_, kFlagIndexPressed);
    Damage(geometry());
    Show();
  }

  if (flags_ & kFlagIndexClick) {
    clear_bit<uint32_t>(flags_, kFlagIndexClick);
    event->Accept();
    clicked_();
    return;
  }
}

void AbstractButton::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void AbstractButton::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void AbstractButton::SetSensitive(bool sensitive) {
  if (sensitive) {
    set_bit<uint32_t>(flags_, kFlagIndexSensitive);
  } else {
    clear_bit<uint32_t>(flags_, kFlagIndexSensitive);
  }
}

}
