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
#include <skland/stock/theme.hpp>

#include "SkCanvas.h"
//#include "SkTypeface.h"
//#include "SkPaint.h"
#include "SkTextBox.h"

namespace skland {

class TitleBar::Button : public AbstractButton {
 public:

  Button();

  virtual ~Button();

 protected:

  virtual void OnDraw(const Context *context) override;

 private:

  static const int kButtonSize = 14;
};

TitleBar::Button::Button()
    : AbstractButton(kButtonSize, kButtonSize) {
}

TitleBar::Button::~Button() {

}

void TitleBar::Button::OnDraw(const Context *context) {
  // override in sub class
}

class TitleBar::CloseButton : public TitleBar::Button {
 public:
  CloseButton()
      : Button() {}

  virtual  ~CloseButton() {}

 protected:

  virtual void OnDraw(const Context *context) final;
};

void TitleBar::CloseButton::OnDraw(const Context *context) {
  Canvas* canvas = context->canvas();
  const Rect &rect = GetGeometry();

  Canvas::ClipGuard guard(canvas, rect);

  canvas->Clear();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(Theme::GetData().title_bar.highlight.foreground.color);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 3.f, rect.center_y() - 3.f,
                   rect.center_x() + 3.f, rect.center_y() + 3.f,
                   paint);
  canvas->DrawLine(rect.center_x() + 3.f, rect.center_y() - 3.f,
                   rect.center_x() - 3.f, rect.center_y() + 3.f,
                   paint);
}

class TitleBar::MaximizeButton : public TitleBar::Button {
 public:
  MaximizeButton()
      : Button() {}

  virtual  ~MaximizeButton() {}

 protected:

  virtual void OnDraw(const Context *context) final;
};

void TitleBar::MaximizeButton::OnDraw(const Context *context) {
  Canvas* canvas = context->canvas();
  const Rect &rect = GetGeometry();

  Canvas::ClipGuard guard(canvas, rect);

  canvas->Clear();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(Theme::GetData().title_bar.highlight.foreground.color);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 4.f, rect.center_y(),
                   rect.center_x() + 4.f, rect.center_y(),
                   paint);
  canvas->DrawLine(rect.center_x(), rect.center_y() - 4.f,
                   rect.center_x(), rect.center_y() + 4.f,
                   paint);
}

class TitleBar::MinimizeButton : public TitleBar::Button {
 public:
  MinimizeButton()
      : Button() {}

  virtual  ~MinimizeButton() {}

 protected:

  virtual void OnDraw(const Context *context) final;
};

void TitleBar::MinimizeButton::OnDraw(const Context *context) {
  Canvas* canvas = context->canvas();
  const Rect &rect = GetGeometry();

  Canvas::ClipGuard guard(canvas, rect);

  canvas->Clear();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(Theme::GetData().title_bar.highlight.foreground.color);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(rect.center_x() - 4.f, rect.center_y(),
                   rect.center_x() + 4.f, rect.center_y(),
                   paint);
}

class TitleBar::FullscreenButton : public TitleBar::Button {
 public:
  FullscreenButton()
      : Button() {}

  virtual  ~FullscreenButton() {}

 protected:

  virtual void OnDraw(const Context *context) final;
};

void TitleBar::FullscreenButton::OnDraw(const Context *context) {
  Canvas* canvas = context->canvas();
  const Rect &rect = GetGeometry();

  Canvas::ClipGuard guard(canvas, rect);

  canvas->Clear();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(rect.center_x(), rect.center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(rect.center_x(), rect.center_y(), 6.5f, paint);
  }

  paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
  paint.SetStyle(Paint::Style::kStyleFill);

  Path path;
  path.MoveTo(-5.f, 0.f);
  path.RelativeLineTo(3.5f, -3.5f);
  path.RelativeLineTo(0.f, 7.f);
  path.Close();

  canvas->Translate(rect.center_x(), rect.center_y());
  canvas->Rotate(-45.f);
  canvas->DrawPath(path, paint);
  canvas->Rotate(180.f);
  canvas->DrawPath(path, paint);
}

TitleBar::TitleBar()
    : AbstractView(),
      close_button_(nullptr),
      maximize_button_(nullptr),
      minimize_button_(nullptr),
      fullscreen_button_(nullptr),
      font_(Typeface::kBold) {
  close_button_ = new CloseButton;
  maximize_button_ = new MaximizeButton;
  minimize_button_ = new MinimizeButton;
  fullscreen_button_ = new FullscreenButton;

  PushBackChild(fullscreen_button_);
  PushBackChild(minimize_button_);
  PushBackChild(maximize_button_);
  PushBackChild(close_button_);
}

TitleBar::~TitleBar() {

}

void TitleBar::SetTitle(const std::string &title) {
  title_ = title;
  Update();
}

AbstractButton *TitleBar::GetButton(ButtonType button_type) const {
  switch (button_type) {
    case kButtonClose: return close_button_;
    case kButtonMaximize: return maximize_button_;
    case kButtonMinimize: return minimize_button_;
    case kButtonFullscreen: return fullscreen_button_;
    default: return nullptr;
  }
}

void TitleBar::OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  int new_y = (GetHeight() - close_button_->GetHeight()) / 2;
  int new_x = kButtonSpace;
  close_button_->MoveTo(new_x, new_y);

  new_x += close_button_->GetWidth() + kButtonSpace;
  maximize_button_->MoveTo(new_x, new_y);

  new_x += maximize_button_->GetWidth() + kButtonSpace;
  minimize_button_->MoveTo(new_x, new_y);

  new_x = GetWidth() - kButtonSpace - fullscreen_button_->GetWidth();
  fullscreen_button_->MoveTo(new_x, new_y);

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

void TitleBar::OnMouseDown(MouseEvent *event) {
  event->Accept();
}

void TitleBar::OnMouseUp(MouseEvent *event) {
  event->Accept();
}

void TitleBar::OnKeyDown(KeyEvent *event) {
  event->Accept();
}

void TitleBar::OnKeyUp(KeyEvent *event) {
  event->Accept();
}

void TitleBar::OnDraw(const Context *context) {
  Paint paint;

  paint.SetColor(Theme::GetData().title_bar.active.foreground.color);
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font_);
  paint.SetTextSize(font_.GetSize());

  float text_width = paint.MeasureText(title_.c_str(), title_.length());

  SkTextBox text_box;
  const Rect &rect = GetGeometry();
  // Put the foreground at the center
  text_box.setBox(rect.l + (rect.width() - text_width) / 2.f,
                  rect.t + 1.f, // move down a little for better look
                  rect.r - (rect.width() - text_width) / 2.f,
                  rect.b);
  text_box.setSpacingAlign(SkTextBox::kCenter_SpacingAlign);
  text_box.setText(title_.c_str(), title_.length(), paint.GetSkPaint());
  text_box.draw(context->canvas()->GetSkCanvas());
}

}