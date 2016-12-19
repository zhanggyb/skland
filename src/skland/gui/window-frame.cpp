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

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include <skland/gui/label.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-window.hpp>

#include <skland/stock/theme.hpp>

#include "SkCanvas.h"

namespace skland {

WindowFrame::CloseButton::CloseButton()
    : AbstractButton() {
  resize(22, 17);
  set_name("CloseButton");
}

WindowFrame::CloseButton::~CloseButton() {

}

Size WindowFrame::CloseButton::GetPreferredSize() const {
  return Size(22, 17);
}

void WindowFrame::CloseButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrame::CloseButton::OnDraw(Canvas *canvas) {
  Color regular(0.9f, 0.33f, 0.33f, 1.f);
  Color down = regular - 50;
  Color hover = regular + 15;

  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(regular);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(down);
    } else {
      paint.SetColor(hover);
    }
  }

  float radii[] = {
      0.f, 0.f, // top-left
      0.f, 0.f, // top-right
      4.f, 4.f, // bottom-right
      4.f, 4.f  // bottom-left
  };
  Path path;
  path.AddRoundRect(geometry(), radii);

  canvas->DrawPath(path, paint);

  if (IsHovered()) {
    paint.SetStrokeWidth(1.75f);
    paint.SetColor(0xFFEBEBEB);
    canvas->DrawLine(center_x() - 2.75f, center_y() - 2.75f, center_x() + 2.75f, center_y() + 2.75f, paint);
    canvas->DrawLine(center_x() + 2.75f, center_y() - 2.75f, center_x() - 2.75f, center_y() + 2.75f, paint);
  }
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
  Color regular(0.25f, 0.8f, 0.25f, 1.f);
  Color down = regular - 50;
  Color hover = regular + 15;
  Color stroke_color = regular - 55;

  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(regular);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(down);
    } else {
      paint.SetColor(hover);
    }
  }
  canvas->DrawCircle(center_x(), center_y(), 6.f, paint);

  paint.SetColor(stroke_color);
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
  Color regular(1.f, 0.75f, 0.2f, 1.f);
  Color down = regular - 50;
  Color hover = regular + 15;
  Color stroke_color = regular - 55;

  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(regular);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(down);
    } else {
      paint.SetColor(hover);
    }
  }
  canvas->DrawCircle(center_x(), center_y(), 6.f, paint);

  paint.SetColor(stroke_color);
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
  if (title_bar_position != kTitleBarNone) {
    CreateWidgets();
  }
}

WindowFrame::~WindowFrame() {
  delete title_;
  delete close_button_;
}

void WindowFrame::OnCloseButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionClose);
}

void WindowFrame::OnMaximizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMaximize);
}

void WindowFrame::OnMinimizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMinimize);
}

void WindowFrame::CreateWidgets() {
  close_button_ = new CloseButton;
  close_button_->clicked().Connect(this, &WindowFrame::OnCloseButtonClicked);

//  minimize_button_ = new MinimizeButton;
//  minimize_button_->clicked().Connect(this, &WindowFrame::OnMinimizeButtonClicked);

//  maximize_button_ = new MaximizeButton;
//  maximize_button_->clicked().Connect(this, &WindowFrame::OnMaximizeButtonClicked);

  title_ = new Label(window()->title(), Font("Arial", Font::kWeightBold));
  title_->SetForebround(0xFFEBEBEB);

  AddWidget(title_);
  AddWidget(close_button_);
//  AddWidget(maximize_button_);
//  AddWidget(minimize_button_);

  LayoutWidgets((int)window()->width(), (int)window()->height());
}

void WindowFrame::Resize(int width, int height) {
  LayoutWidgets(width, height);
}

void WindowFrame::LayoutWidgets(int width, int height) {
  int x = 11;
  int y = 0;

  const int space = 5;

  close_button_->SetPosition(x, y);

//  x += space + close_button_->width();
//  y = (title_bar_size() - minimize_button_->height()) / 2;
//  minimize_button_->SetPosition(x, y);

//  x += space + minimize_button_->width();
//  y = (title_bar_size() - maximize_button_->height()) / 2;
//  maximize_button_->SetPosition(x, y);

//  x += space + maximize_button_->width();
//  y = 0;
//  int w = width - x;

//  if (w > 0) {
    title_->SetPosition(0, 0);
    title_->Resize((int)window()->width(), title_bar_size());
//  }
}

void WindowFrame::Draw(Canvas *canvas) {
  canvas->Clear();

  DrawShadow(canvas);

  Paint paint;
  paint.SetColor(0xFF555555);
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

  paint.SetColor(0xFFEBEBEB);
  canvas->DrawRect(GetClientGeometry(), paint);

  canvas->Flush();
}

int WindowFrame::GetMouseLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->surface_x()), y = static_cast<int>(event->surface_y());

  // TODO: maximized or frameless

  if (x < (Theme::shadow_margin_left() - kResizingMargin.left))
    hlocation = kExterior;
  else if (x < Theme::shadow_margin_left() + kResizingMargin.left)
    hlocation = kResizeLeft;
  else if (x < Theme::shadow_margin_left() + window()->width() - kResizingMargin.right)
    hlocation = kInterior;
  else if (x < Theme::shadow_margin_left() + window()->width() + kResizingMargin.right)
    hlocation = kResizeRight;
  else
    hlocation = kExterior;

  if (y < (Theme::shadow_margin_top() - kResizingMargin.top))
    vlocation = kExterior;
  else if (y < Theme::shadow_margin_top() + kResizingMargin.top)
    vlocation = kResizeTop;
  else if (y < Theme::shadow_margin_top() + window()->height() - kResizingMargin.bottom)
    vlocation = kInterior;
  else if (y < Theme::shadow_margin_top() + window()->height() + kResizingMargin.bottom)
    vlocation = kResizeBottom;
  else
    vlocation = kExterior;

  location = vlocation | hlocation;
  if (location & kExterior)
    location = kExterior;

  if (location == kInterior &&
      y < Theme::shadow_margin_top() + title_bar_size())
    location = kTitleBar;
  else if (location == kInterior)
    location = kClientArea;

  return location;
}

void WindowFrame::DrawShadow(Canvas *canvas) {

  const float rad = 20.f; // The spread radius

  const float offset_x = Theme::shadow_offset_x();
  const float offset_y = Theme::shadow_offset_y();

  // shadow map
  SkCanvas *c = canvas->sk_canvas();
  sk_sp<SkImage> image = SkImage::MakeFromRaster(*Theme::shadow_pixmap(), nullptr, nullptr);

  // top-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 0,
                                    2 * Theme::shadow_radius(), 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(-rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // top
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::shadow_radius(), 0,
                                    250 - 2 * Theme::shadow_radius(), 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(rad + offset_x, -rad + offset_y,
                                    window()->width() - 2 * rad, 2 * rad),
                   nullptr);

  // top-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 0,
                                    250, 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(window()->width() - rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(-rad + offset_x, rad + offset_y,
                                    2 * rad, window()->height() - 2 * rad),
                   nullptr);

  // bottom-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 250 - 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(-rad + offset_x, window()->height() - rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // bottom
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250 - 2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(rad + offset_x, window()->height() - rad + offset_y,
                                    window()->width() - 2 * rad, 2 * rad),
                   nullptr);

  // bottom-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250, 250),
                   SkRect::MakeXYWH(window()->width() - rad + offset_x,
                                    window()->height() - rad + offset_y,
                                    2 * rad,
                                    2 * rad),
                   nullptr);

  // right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 2 * Theme::shadow_radius(),
                                    250, 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(window()->width() - rad + offset_x, rad + offset_y,
                                    2 * rad, window()->height() - 2 * rad),
                   nullptr);

}

}
