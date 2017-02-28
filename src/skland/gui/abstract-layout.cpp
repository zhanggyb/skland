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

#include <skland/gui/abstract-layout.hpp>

#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/gui/context.hpp>
#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include "internal/abstract-view-iterators.hpp"

//#ifdef DEBUG
#include <cstdlib>
#include <skland/gui/timer.hpp>
//#endif

namespace skland {

AbstractLayout::AbstractLayout(const Margin &padding)
    : AbstractView(), padding_(padding) {

}

AbstractLayout::~AbstractLayout() {

}

Size AbstractLayout::GetMinimalSize() const {
  return Size(padding_.lr(), padding_.tb());
}

Size AbstractLayout::GetPreferredSize() const {
  return Size(400, 300);
}

Size AbstractLayout::GetMaximalSize() const {
  return Size(65536, 65536);
}

void AbstractLayout::AddView(AbstractView *view) {
  Iterator it(view);

  // TODO: check if view already in this layout
  if (it.parent() == this)
    return;

  if (it.parent()) {
    static_cast<AbstractLayout *>(it.parent())->RemoveView(view);
  }

  DBG_ASSERT(nullptr == it.parent());
  InsertChild(view);
  OnViewAdded(view);
}

void AbstractLayout::RemoveView(AbstractView *view) {
  Iterator it(view);

  if (it.parent() != this)
    return;

  RemoveChild(view);
  OnViewRemoved(view);
}

void AbstractLayout::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseLeave(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseMove(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseButton(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnKeyboardKey(KeyEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnDraw(const Context *context) {
//#ifdef DEBUG
  srand(Timer::GetMicroSeconds());
  float r = rand() % 255 / 255.f;
  float g = rand() % 255 / 255.f;
  float b = rand() % 255 / 255.f;

  Paint paint;
  paint.SetColor(Color(r, g, b, 0.25));
  context->canvas()->DrawRect(geometry(), paint);
//#endif
}

}