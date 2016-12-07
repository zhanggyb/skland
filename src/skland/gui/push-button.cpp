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
#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/gui/mouse-event.hpp>

namespace skland {

PushButton::PushButton()
    : AbstractButton(), hover_(false) {
  resize(90, 20);
  set_name("CloseButton");
}

PushButton::~PushButton() {

}

Size PushButton::GetPreferredSize() const {
  return Size(90, 20);
}

void PushButton::OnMouseEnter(MouseEvent *event) {
  fprintf(stderr, "enter: %d %d\n", (int)event->window_x(), (int)event->window_y());
  hover_ = true;
  Show();
  event->Accept();
}

void PushButton::OnMouseLeave(MouseEvent *event) {
  fprintf(stderr, "leave: %d %d\n", (int)event->window_x(), (int) event->window_y());
  hover_ = false;
  Show();
  event->Accept();
}

void PushButton::OnResize(int width, int height) {
  resize(width, height);
  Show();
}

void PushButton::OnDraw(const Canvas *canvas) {

  Paint paint;

  if (hover_) {
    paint.SetColor(Color(0.75f, 0.75f, 0.75f, 1.f));
  } else {
    paint.SetColor(Color(0.85f, 0.85f, 0.15f, 1.f));
  }

  canvas->DrawRectangle(x(), y(), width(), height(), paint);

  /*
  canvas.SetOperator(CAIRO_OPERATOR_SOURCE);
  canvas.SetColor(0.75, 0.75, 0.75, 1.);
  canvas.RoundedRectangle(x() + 0.5,
                          y() + 0.5,
                          x() + width() - 0.5,
                          y() + height() - 0.5,
                          4.);
  canvas.Fill();
  canvas.SetColor(0.1, 0.1, 0.1, 1.);
  canvas.RoundedRectangle(x() + 0.5,
                          y() + 0.5,
                          x() + width() - 0.5,
                          y() + height() - 0.5,
                          4.);
  canvas.SetLineWidth(1.);
  canvas.Stroke();
   */
  canvas->Flush();
}

}