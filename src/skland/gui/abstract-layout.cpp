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
#include "internal/abstract-view-private.hpp"

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

void AbstractLayout::AddView(AbstractView *view) {
  DBG_ASSERT(view->p_->parent == view->p_->layout);

  if (view->p_->layout == this) return;

  if (view->p_->layout) view->p_->layout->RemoveView(view);

  DBG_ASSERT(nullptr == view->p_->layout);
  DBG_ASSERT(nullptr == view->p_->parent);

  InsertChild(view);
  view->p_->layout = this;

  OnViewAdded(view);
}

void AbstractLayout::RemoveView(AbstractView *view) {
  DBG_ASSERT(view->p_->parent == view->p_->layout);

  if (view->p_->layout != this) return;

  RemoveChild(view);
  view->p_->layout = nullptr;

  OnViewRemoved(view);
}

void AbstractLayout::OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  if (dirty_flag) Update();
  else CancelUpdate();
}

void AbstractLayout::OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  OnLayout(dirty_flag,
           static_cast<int>(new_geometry.left),
           static_cast<int>(new_geometry.top),
           static_cast<int>(new_geometry.right),
           static_cast<int>(new_geometry.bottom));
}

void AbstractLayout::OnChildAdded(AbstractView *view) {
  // Use OnViewAdded() in layout class
}

void AbstractLayout::OnChildRemoved(AbstractView *view) {
  // Use OnViewRemoved() in layout class
}

void AbstractLayout::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseLeave() {

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
  context->canvas()->DrawRect(GetGeometry(), paint);
//#endif
}

}