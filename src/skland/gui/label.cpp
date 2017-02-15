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

#include <skland/gui/context.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include "SkTypeface.h"
#include "SkPaint.h"
#include "SkTextBox.h"

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
}

Label::~Label() {

}

void Label::SetForeground(const Color &color) {
  if (foreground_ != color) {
    foreground_ = color;
    Update();
  }
}

void Label::SetBackground(const Color &color) {
  if (background_ != color) {
    background_ = color;
    Update();
  }
}

void Label::SetFont(const Font &font) {
  font_ = font;
  Update();
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

void Label::OnDraw(const Context *context) {
  Paint paint;
  paint.SetColor(background_);
  context->canvas()->DrawRect(geometry(), paint);

  paint.SetColor(foreground_);
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font_);
  paint.SetTextSize(font_.GetSize());

  float text_width = paint.MeasureText(text_.c_str(), text_.length());

  SkTextBox text_box;
  // Put the text at the center
  text_box.setBox((geometry().l + geometry().r - text_width) / 2.f,
                  geometry().t + 1.f, // move down a little for better look
                  (geometry().r - geometry().l + text_width) / 2.f,
                  geometry().b);
  text_box.setSpacingAlign(SkTextBox::kCenter_SpacingAlign);
  text_box.setText(text_.c_str(), text_.length(), *paint.sk_paint());
  text_box.draw(context->canvas()->sk_canvas());
}

}
