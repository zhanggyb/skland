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

#include <skland/gui/title-bar.hpp>

#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/context.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include "SkCanvas.h"
//#include "SkTypeface.h"
//#include "SkPaint.h"
#include "SkTextBox.h"

namespace skland {

CloseButton::CloseButton()
    : AbstractButton(kButtonSize, kButtonSize),
      foreground_(0xFF444444),
      background_(0xFF999999) {
}

CloseButton::~CloseButton() {

}

Size CloseButton::GetPreferredSize() const {
  return Size(kButtonSize, kButtonSize);
}

void CloseButton::SetForeground(const Color &color) {
  foreground_ = color;
  Update();
}

void CloseButton::SetBackground(const Color &color) {
  background_ = color;
  Update();
}

void CloseButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Save();
  canvas->ClipRect(GetGeometry());
  canvas->Clear();

  const Rect &rect = GetGeometry();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(background_);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(foreground_);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(foreground_);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 3.f, rect.center_y() - 3.f,
                   rect.center_x() + 3.f, rect.center_y() + 3.f,
                   paint);
  canvas->DrawLine(rect.center_x() + 3.f, rect.center_y() - 3.f,
                   rect.center_x() - 3.f, rect.center_y() + 3.f,
                   paint);

  canvas->Restore();
}

MaximizeButton::MaximizeButton()
    : AbstractButton(kButtonSize, kButtonSize),
      foreground_(0xFF444444),
      background_(0xFF999999) {
}

MaximizeButton::~MaximizeButton() {

}

void MaximizeButton::SetForeground(const Color &color) {
  foreground_ = color;
  Update();
}

void MaximizeButton::SetBackground(const Color &color) {
  background_ = color;
  Update();
}

Size MaximizeButton::GetPreferredSize() const {
  return Size(kButtonSize, kButtonSize);
}

void MaximizeButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Save();
  canvas->ClipRect(GetGeometry());
  canvas->Clear();

  const Rect &rect = GetGeometry();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(background_);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(foreground_);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(foreground_);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 4.f, rect.center_y(),
                   rect.center_x() + 4.f, rect.center_y(),
                   paint);
  canvas->DrawLine(rect.center_x(), rect.center_y() - 4.f,
                   rect.center_x(), rect.center_y() + 4.f,
                   paint);

  canvas->Restore();
}

MinimizeButton::MinimizeButton()
    : AbstractButton(kButtonSize, kButtonSize),
      foreground_(0xFF444444),
      background_(0xFF999999) {
}

MinimizeButton::~MinimizeButton() {

}

Size MinimizeButton::GetPreferredSize() const {
  return Size(kButtonSize, kButtonSize);
}

void MinimizeButton::SetForeground(const Color &color) {
  foreground_ = color;
  Update();
}

void MinimizeButton::SetBackground(const Color &color) {
  background_ = color;
  Update();
}

void MinimizeButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Save();
  canvas->ClipRect(GetGeometry());
  canvas->Clear();

  const Rect &rect = GetGeometry();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(background_);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(foreground_);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(foreground_);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 4.f, rect.center_y(),
                   rect.center_x() + 4.f, rect.center_y(),
                   paint);

  canvas->Restore();
}

TitleBar::TitleBar()
    : AbstractView(),
      close_button_(nullptr),
      maximize_button_(nullptr),
      minimize_button_(nullptr),
      font_(Typeface::kBold),
      foreground_(0xFF444444) {
  close_button_ = new CloseButton;
  maximize_button_ = new MaximizeButton;
  minimize_button_ = new MinimizeButton;

  PushBackChild(minimize_button_);
  PushBackChild(maximize_button_);
  PushBackChild(close_button_);
}

TitleBar::~TitleBar() {

}

void TitleBar::SetForeground(const Color &color) {
  foreground_ = color;
  Update();
}

void TitleBar::SetTitle(const std::string &title) {
  title_ = title;
  Update();
}

void TitleBar::OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  int new_y = (GetHeight() - close_button_->GetHeight()) / 2;
  int new_x = kButtonSpace;
  close_button_->MoveTo(new_x, new_y);

  new_x += close_button_->GetWidth() + kButtonSpace;
  maximize_button_->MoveTo(new_x, new_y);

  new_x += maximize_button_->GetWidth() + kButtonSpace;
  minimize_button_->MoveTo(new_x, new_y);

  RecursiveUpdate();
}

void TitleBar::OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {

}

void TitleBar::OnLayout(int dirty_flag, int left, int top, int right, int bottom) {

}

void TitleBar::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void TitleBar::OnMouseLeave() {

}

void TitleBar::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void TitleBar::OnMouseButton(MouseEvent *event) {
  event->Accept();
}

void TitleBar::OnKeyboardKey(KeyEvent *event) {
  event->Accept();
}

void TitleBar::OnDraw(const Context *context) {
  Paint paint;

  paint.SetColor(foreground_);
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font_);
  paint.SetTextSize(font_.GetSize());

  float text_width = paint.MeasureText(title_.c_str(), title_.length());

  SkTextBox text_box;
  const Rect &rect = GetGeometry();
  // Put the text at the center
  text_box.setBox(rect.l + (rect.width() - text_width) / 2.f,
                  rect.t + 1.f, // move down a little for better look
                  rect.r - (rect.width() - text_width) / 2.f,
                  rect.b);
  text_box.setSpacingAlign(SkTextBox::kCenter_SpacingAlign);
  text_box.setText(title_.c_str(), title_.length(), paint.GetSkPaint());
  text_box.draw(context->canvas()->GetSkCanvas());
}

}