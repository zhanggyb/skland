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

#include <skland/gui/label.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include "SkTypeface.h"
#include "SkPaint.h"

namespace skland {

Label::Label(const std::string &text, const Font &font)
    : Label(text, 200, 24, font) {
}

Label::Label(const std::string &text, int width, int height, const Font &font)
    : AbstractWidget(width, height),
      text_(text),
      foreground_(0.2f, 0.2f, 0.2f),
      background_(0.f, 0.f, 0.f, 0.f),
      font_(font) {
  set_name("Label");
}

Label::~Label() {

}

void Label::SetForebround(const Color &color) {
  if (foreground_ != color) {
    foreground_ = color;
    Show();
  }
}

void Label::SetBackground(const Color &color) {
  if (background_ != color) {
    background_ = color;
    Show();
  }
}

void Label::OnResize(int width, int height) {
  resize(width, height);
}

void Label::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void Label::OnMouseLeave(MouseEvent *event) {
  event->Ignore();
}

void Label::OnMouseMove(MouseEvent *event) {
  event->Ignore();
}

void Label::OnMouseButton(MouseEvent *event) {
  event->Ignore();
}

void Label::OnKeyboardKey(KeyEvent *event) {
  event->Ignore();
}

void Label::OnDraw(Canvas *canvas) {

  Paint paint;
  paint.SetColor(background_);
  canvas->DrawRectangle(x(), y(), width(), height(), paint);

  paint.SetColor(foreground_);
  paint.SetAntiAlias(true);
  paint.SetTextSize(12.f);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font_);

  canvas->DrawText(text_.c_str(), text_.length(), x() + 5.f, y() + 16.f, paint);
}

}
