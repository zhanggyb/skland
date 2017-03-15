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
#include <skland/graphic/text-box.hpp>

namespace skland {

PushButton::PushButton(const std::string &text)
    : AbstractButton(text) {
}

PushButton::~PushButton() {

}

Size PushButton::GetPreferredSize() const {
  return Size(90, 20);
}

void PushButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();

  Color regular(0.95f, 0.55f, 0.1f);
  Color down = regular - 50;
  Color hover = regular + 25;

  const Rect &rect = GetGeometry();

  Paint paint;
  paint.SetColor(regular);
  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(down);
    } else {
      paint.SetColor(hover);
    }
  }

  canvas->DrawRect(rect, paint);

  const Font &font = GetFont();
  const std::string &text = GetText();

  paint.SetColor(0xFF444444);
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font);
  paint.SetTextSize(font.GetSize());

  float text_width = paint.MeasureText(text.c_str(), text.length());

  TextBox text_box;
  // Put the text at the center
  text_box.SetBox(rect.l + (rect.width() - text_width) / 2.f,
                  rect.t + 1.f, // move down a little for better look
                  rect.r - (rect.width() - text_width) / 2.f,
                  rect.b);
  text_box.SetSpacingAlign(TextBox::kSpacingAlignCenter);
  text_box.SetText(text.c_str(), text.length(), paint);
  text_box.Draw(*canvas);
}

}