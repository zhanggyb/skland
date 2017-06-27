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

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include "internal/abstract-view_iterators.hpp"

//#ifdef DEBUG
#include <cstdlib>
#include <skland/gui/timer.hpp>
#include <skland/core/assert.hpp>
//#endif

namespace skland {
namespace gui {

using core::RectF;
using core::ColorF;

using graphic::Paint;
using graphic::Canvas;

AbstractLayout::AbstractLayout(const core::Padding &padding)
    : AbstractView() {
  p_->padding = padding;
  p_->need_redraw = false;
}

AbstractLayout::~AbstractLayout() {

}

void AbstractLayout::AddView(AbstractView *view) {
  _ASSERT(view->p_->parent == view->p_->layout);

  if (view->p_->layout == this) return;

  if (view->p_->layout) view->p_->layout->RemoveView(view);

  _ASSERT(nullptr == view->p_->layout);
  _ASSERT(nullptr == view->p_->parent);

  InsertChild(view);
  Layout();
}

void AbstractLayout::RemoveView(AbstractView *view) {
  _ASSERT(view->p_->parent == view->p_->layout);

  if (view->p_->layout != this) return;

  RemoveChild(view);
  Layout();
}

void AbstractLayout::Layout() {
  p_->need_layout = true;
  Update();
}

void AbstractLayout::OnConfigureGeometry(int dirty_flag, const RectF &old_geometry, const RectF &new_geometry) {
  if (dirty_flag) {
    Layout();
  } else {
    p_->need_layout = false;
    Update(false);
  }
}

void AbstractLayout::OnGeometryChange(int dirty_flag, const RectF &old_geometry, const RectF &new_geometry) {
  p_->need_layout = true;
}

void AbstractLayout::OnChildAdded(AbstractView *view) {
  view->p_->layout = this;
  OnViewAdded(view);
}

void AbstractLayout::OnChildRemoved(AbstractView *view) {
  view->p_->layout = nullptr;
  OnViewRemoved(view);
}

void AbstractLayout::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseLeave() {

}

void AbstractLayout::OnMouseMove(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseDown(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnMouseUp(MouseEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnKeyDown(KeyEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnKeyUp(KeyEvent *event) {
  event->Ignore();
}

void AbstractLayout::OnDraw(const Context *context) {
//#ifdef DEBUG
  srand(Timer::GetClockTime());
  float r = rand() % 255 / 255.f;
  float g = rand() % 255 / 255.f;
  float b = rand() % 255 / 255.f;

  Paint paint;
  paint.SetColor(ColorF(r, g, b, 0.25));
  context->canvas()->DrawRect(GetGeometry(), paint);
//#endif
}

} // namespace gui
} // namespace skland
