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

#include <skland/gui/push-button.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/context.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

namespace skland {

PushButton::PushButton()
    : AbstractButton(90, 20) {
}

PushButton::~PushButton() {

}

Size PushButton::GetPreferredSize() const {
  return Size(90, 20);
}

void PushButton::OnGeometryUpdate(const Rect &new_geometry) {

}

void PushButton::OnDraw(const Context *context) {
  Color regular(0.95f, 0.55f, 0.1f);
  Color down = regular - 50;
  Color hover = regular + 25;

  Paint paint;
  paint.SetColor(regular);
  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(down);
    } else {
      paint.SetColor(hover);
    }
  }

  context->canvas()->DrawRect(GetGeometry(), paint);
}

}