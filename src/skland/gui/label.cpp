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
#include <skland/graphic/text-box.hpp>

#include <skland/gui/context.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

namespace skland {

Label::Label(const std::string &text, const Font &font)
    : Label(text, 200, 24, font) {
}

Label::Label(const std::string &text, int width, int height, const Font &font)
    : AbstractView(width, height),
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

void Label::OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  if (dirty_flag) {
    Update();
  } else {
    CancelUpdate();
  }
}

void Label::OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {

}

void Label::OnLayout(int dirty_flag, int left, int top, int right, int bottom) {

}

void Label::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void Label::OnMouseLeave() {

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
  const Rect &rect = GetGeometry();
  Canvas::ClipGuard guard(context->canvas(), rect);

  Paint paint;
  paint.SetColor(background_);
  context->canvas()->DrawRect(rect, paint);

  paint.SetColor(foreground_);
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font_);
  paint.SetTextSize(font_.GetSize());

  float text_width = paint.MeasureText(text_.c_str(), text_.length());

  TextBox text_box;
  // Put the text at the center
  text_box.SetBox(rect.l + (rect.width() - text_width) / 2.f,
                  rect.t + 1.f, // move down a little for better look
                  rect.r - (rect.width() - text_width) / 2.f,
                  rect.b);
  text_box.SetSpacingAlign(TextBox::kSpacingAlignCenter);
  text_box.SetText(text_.c_str(), text_.length(), paint);
  text_box.Draw(*context->canvas());
}

}
