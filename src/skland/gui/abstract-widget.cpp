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

#include <skland/gui/abstract-widget.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include <skland/gui/abstract-surface.hpp>

#include "internal/redraw-task.hpp"

namespace skland {

AbstractWidget::AbstractWidget()
    : AbstractView(200, 200) {

}

AbstractWidget::AbstractWidget(int width, int height)
    : AbstractWidget() {
  resize(width, height);
}

AbstractWidget::~AbstractWidget() {
}

Size AbstractWidget::GetMinimalSize() const {
  return Size(0, 0);
}

Size AbstractWidget::GetPreferredSize() const {
  return Size(200, 200);
}

Size AbstractWidget::GetMaximalSize() const {
  return Size(65536, 65536);
}

void AbstractWidget::OnShow() {
  AbstractSurface *surface = GetSurface();
  if (surface && surface->canvas()) {
    static_cast<RedrawTask *>(redraw_task().get())->canvas = surface->canvas().get();
    AddRedrawTask(redraw_task().get());
    surface->Commit();
  }
}

Widget::~Widget() {

}

void Widget::OnResize(int width, int height) {

}

void Widget::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void Widget::OnMouseLeave(MouseEvent *event) {
  event->Accept();
}

void Widget::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void Widget::OnMouseButton(MouseEvent *event) {
  event->Accept();
}

void Widget::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void Widget::OnDraw(Canvas *canvas) {
  Paint paint;
  paint.SetColor(Color(0.95f, 0.95f, 0.95f, 1.f));

  canvas->DrawRect(geometry(), paint);
}

}
