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

#include <skland/gui/window-frame.hpp>

#include <stdlib.h>

#include <skland/gui/label.hpp>
#include <skland/gui/mouse-event.hpp>

#include <skland/stock/theme.hpp>

#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

namespace skland {

WindowFrame::CloseButton::CloseButton()
    : AbstractButton() {
  resize(14, 14);
  set_name("CloseButton");
}

WindowFrame::CloseButton::~CloseButton() {

}

Size WindowFrame::CloseButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrame::CloseButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrame::CloseButton::OnDraw(Canvas *canvas) {
  Color fill_color(1.f, 0.35f, 0.35f, 1.f);
  Color stroke_color = fill_color - 55;

  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(fill_color);

  canvas->DrawCircle(center_x(), center_y(), 6.f, paint);

  paint.SetColor(stroke_color);

  if (IsHovered()) {
    canvas->DrawCircle(center_x(), center_y(), 2.f, paint);
  }

  paint.SetStyle(Paint::kStyleStroke);
  paint.SetStrokeWidth(0.5f);
  canvas->DrawCircle(center_x(), center_y(), 5.5f, paint);
}

WindowFrame::MaximizeButton::MaximizeButton()
    : AbstractButton() {
  resize(14, 14);
  set_name("MaximizeButton");
}

WindowFrame::MaximizeButton::~MaximizeButton() {

}

Size WindowFrame::MaximizeButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrame::MaximizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrame::MaximizeButton::OnDraw(Canvas *canvas) {
  Color fill_color(0.25f, 0.8f, 0.25f, 1.f);
  Color stroke_color = fill_color - 55;

  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(fill_color);

  canvas->DrawCircle(center_x(), center_y(), 6.f, paint);

  paint.SetColor(stroke_color);

  if (IsHovered()) {
    canvas->DrawCircle(center_x(), center_y(), 2.f, paint);
  }

  paint.SetStyle(Paint::kStyleStroke);
  paint.SetStrokeWidth(0.5f);
  canvas->DrawCircle(center_x(), center_y(), 5.5f, paint);
}

WindowFrame::MinimizeButton::MinimizeButton()
    : AbstractButton() {
  resize(14, 14);
  set_name("MinimizeButton");
}

WindowFrame::MinimizeButton::~MinimizeButton() {

}

Size WindowFrame::MinimizeButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrame::MinimizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrame::MinimizeButton::OnDraw(Canvas *canvas) {
  Color fill_color(1.f, 0.75f, 0.2f, 1.f);
  Color stroke_color = fill_color - 55;

  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(fill_color);

  canvas->DrawCircle(center_x(), center_y(), 6.f, paint);

  paint.SetColor(stroke_color);

  if (IsHovered()) {
    canvas->DrawCircle(center_x(), center_y(), 2.f, paint);
  }

  paint.SetStyle(Paint::kStyleStroke);
  paint.SetStrokeWidth(0.5f);
  canvas->DrawCircle(center_x(), center_y(), 5.5f, paint);
}

//-------------------------

WindowFrame::WindowFrame(AbstractWindow *window,
                         int border,
                         TitleBarPosition title_bar_position,
                         int title_bar_size)
    : AbstractWindowFrame(window, border, title_bar_position, title_bar_size),
      close_button_(nullptr),
      maximize_button_(nullptr),
      minimize_button_(nullptr),
      title_(nullptr) {
  srand(time(NULL));
  unsigned char r = rand() % 127 + 128;
  unsigned char g = rand() % 127 + 128;
  unsigned char b = rand() % 127 + 128;

  background_ = Color::FromUCharRGBA(r, g, b);
  foreground_ = Color::ReverseRGBFrom(background_);

  if (title_bar_position != kTitleBarNone) {
    CreateWidgets();
  }
}

WindowFrame::~WindowFrame() {
  delete title_;
  delete close_button_;
}

void WindowFrame::OnCloseButtonClicked(SLOT /* slot */) {
  EmitAction(kWindowClose);
}

void WindowFrame::OnMaximizeButtonClicked(SLOT /* slot */) {
  EmitAction(kWindowMaximize);
}

void WindowFrame::OnMinimizeButtonClicked(SLOT /* slot */) {
  EmitAction(kWindowMinimize);
}

void WindowFrame::CreateWidgets() {
  close_button_ = new CloseButton;
  close_button_->clicked().Connect(this, &WindowFrame::OnCloseButtonClicked);

  minimize_button_ = new MinimizeButton;
  minimize_button_->clicked().Connect(this, &WindowFrame::OnMinimizeButtonClicked);

  maximize_button_ = new MaximizeButton;
  maximize_button_->clicked().Connect(this, &WindowFrame::OnMaximizeButtonClicked);

  title_ = new Label(window()->title());
  title_->SetForebround(foreground_);

  AddWidget(title_);
  AddWidget(maximize_button_);
  AddWidget(minimize_button_);
  AddWidget(close_button_);

  LayoutWidgets(window()->width(), window()->height());
}

void WindowFrame::Resize(int width, int height) {
  LayoutWidgets(width, height);
}

void WindowFrame::LayoutWidgets(int width, int height) {
  int x = 0;
  int y = 0;

  const int space = 5;

  x += space;
  y = (title_bar_size() - close_button_->height()) / 2;
  close_button_->SetPosition(x, y);

  x += space + close_button_->width();
  y = (title_bar_size() - minimize_button_->height()) / 2;
  minimize_button_->SetPosition(x, y);

  x += space + minimize_button_->width();
  y = (title_bar_size() - maximize_button_->height()) / 2;
  maximize_button_->SetPosition(x, y);

  x += space + maximize_button_->width();
  y = 0;
  int w = width - x;

  if (w > 0) {
    title_->SetPosition(x, y);
    title_->Resize(w, title_bar_size());
  }
}

void WindowFrame::Draw(Canvas *canvas) {
  canvas->Clear();

  Paint paint;
  paint.SetColor(background_.argb());
  paint.SetAntiAlias(true);

  float radii[] = {
      7.f, 7.f, // top-left
      7.f, 7.f, // top-right
      4.f, 4.f, // bottom-right
      4.f, 4.f  // bottom-left
  };
  Path path;
  path.AddRoundRect(window()->geometry(), radii);

  canvas->DrawPath(path, paint);
  canvas->Flush();
}

int WindowFrame::GetPointerLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->surface_x()), y = static_cast<int>(event->surface_y());

  // TODO: maximized or frameless

  if (x < (kShadowMargin.left - kResizingMargin.left))
    hlocation = kExterior;
  else if (x < kShadowMargin.left + kResizingMargin.left)
    hlocation = kResizeLeft;
  else if (x < kShadowMargin.left + window()->width() - kResizingMargin.right)
    hlocation = kInterior;
  else if (x < kShadowMargin.left + window()->width() + kResizingMargin.right)
    hlocation = kResizeRight;
  else
    hlocation = kExterior;

  if (y < (kShadowMargin.top - kResizingMargin.top))
    vlocation = kExterior;
  else if (y < kShadowMargin.top + kResizingMargin.top)
    vlocation = kResizeTop;
  else if (y < kShadowMargin.top + window()->height() - kResizingMargin.bottom)
    vlocation = kInterior;
  else if (y < kShadowMargin.top + window()->height() + kResizingMargin.bottom)
    vlocation = kResizeBottom;
  else
    vlocation = kExterior;

  location = vlocation | hlocation;
  if (location & kExterior)
    location = kExterior;

  if (location == kInterior &&
      y < kShadowMargin.top + title_bar_size())
    location = kTitleBar;
  else if (location == kInterior)
    location = kClientArea;

  return location;
}

}
