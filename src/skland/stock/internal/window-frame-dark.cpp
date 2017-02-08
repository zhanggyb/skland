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

#include "window-frame-dark.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include <skland/gui/label.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-window.hpp>

#include <skland/gui/context.hpp>
#include "SkCanvas.h"

namespace skland {

WindowFrameDark::CloseButton::CloseButton()
    : AbstractButton() {
  resize(14, 14);
}

WindowFrameDark::CloseButton::~CloseButton() {
}

Size WindowFrameDark::CloseButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrameDark::CloseButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameDark::CloseButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->GetCanvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Color regular(0.95f, 0.55f, 0.f);
  // Color outline(0.78f, 0.28f, 0.28f);
  Color outline(0xFFCCCCCC);
  Color down = regular - 75;
  Color hover = regular + 15;

  Paint paint;

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(down);
    } else {
      paint.SetColor(hover);
    }
  } else {
    paint.SetColor(outline);
  }

  paint.SetStyle(Paint::Style::kStyleStroke);
  paint.SetAntiAlias(true);
  paint.SetStrokeWidth(1.f);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.5f, paint);

  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(geometry().center_x() - 2.5f, geometry().center_y() - 2.5f,
                   geometry().center_x() + 2.5f, geometry().center_y() + 2.5f,
                   paint);
  canvas->DrawLine(geometry().center_x() + 2.5f, geometry().center_y() - 2.5f,
                   geometry().center_x() - 2.5f, geometry().center_y() + 2.5f,
                   paint);
  canvas->Restore();
}

WindowFrameDark::MaximizeButton::MaximizeButton()
    : AbstractButton() {
  resize(14, 14);
}

WindowFrameDark::MaximizeButton::~MaximizeButton() {

}

Size WindowFrameDark::MaximizeButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrameDark::MaximizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameDark::MaximizeButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->GetCanvas();

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
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.f, paint);

  paint.SetColor(stroke_color);
  paint.SetStyle(Paint::kStyleStroke);
  paint.SetStrokeWidth(0.5f);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 5.5f, paint);
}

WindowFrameDark::MinimizeButton::MinimizeButton()
    : AbstractButton() {
  resize(14, 14);
}

WindowFrameDark::MinimizeButton::~MinimizeButton() {

}

Size WindowFrameDark::MinimizeButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrameDark::MinimizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameDark::MinimizeButton::OnDraw(const Context *context) {
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
  context->GetCanvas()->DrawCircle(geometry().center_x(), geometry().center_y(), 6.f, paint);

  paint.SetColor(stroke_color);
  paint.SetStyle(Paint::kStyleStroke);
  paint.SetStrokeWidth(0.5f);
  context->GetCanvas()->DrawCircle(geometry().center_x(), geometry().center_y(), 5.5f, paint);
}

//-------------------------

WindowFrameDark::WindowFrameDark()
    : AbstractWindowFrame(),
      close_button_(nullptr),
      maximize_button_(nullptr),
      minimize_button_(nullptr),
      title_(nullptr) {
  set_border(0);
  set_title_bar_size(22);
  set_title_bar_position(kTitleBarTop);
}

WindowFrameDark::~WindowFrameDark() {
  delete title_;
  delete close_button_;
}

void WindowFrameDark::OnCloseButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionClose);
}

void WindowFrameDark::OnMaximizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMaximize);
}

void WindowFrameDark::OnMinimizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMinimize);
}

void WindowFrameDark::CreateWidgets() {
  close_button_ = new CloseButton;
  close_button_->clicked().Connect(this, &WindowFrameDark::OnCloseButtonClicked);

//  minimize_button_ = new MinimizeButton;
//  minimize_button_->clicked().Connect(this, &DefaultWindowFrame::OnMinimizeButtonClicked);

//  maximize_button_ = new MaximizeButton;
//  maximize_button_->clicked().Connect(this, &DefaultWindowFrame::OnMaximizeButtonClicked);

  title_ = new Label(window()->title());
  title_->SetForeground(0xFFBBBBBB);
  title_->SetFont(Font(Typeface::kBold));

  AddWidget(title_);  // put the title below other widgets
  AddWidget(close_button_);
//  AddWidget(maximize_button_);
//  AddWidget(minimize_button_);

  LayoutWidgets(window()->width(), window()->height());
}

void WindowFrameDark::OnResize(int width, int height) {
  LayoutWidgets(width, height);
}

void WindowFrameDark::LayoutWidgets(int width, int height) {
  title_->MoveTo(0, 0);
  title_->Resize(window()->width(), title_bar_size());

  int y = (title_bar_size() - (int) close_button_->geometry().height()) / 2;
  int x = y + 1;
  close_button_->MoveTo(x, y);
}

void WindowFrameDark::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->GetCanvas();
  canvas->Clear();

  float radii[] = {
      7.f, 7.f, // top-left
      7.f, 7.f, // top-right
      4.f, 4.f, // bottom-right
      4.f, 4.f  // bottom-left
  };
  Path path;
  path.AddRoundRect(window()->geometry(), radii);

  canvas->Save();
  canvas->ClipPath(path, kClipDifference, true);
  DrawShadow(canvas.get());
  canvas->Restore();

  Paint paint;
  paint.SetAntiAlias(true);

  path.Reset();
  path.AddRoundRect(window()->geometry(), radii);
  paint.SetColor(0xEF202020);
  canvas->DrawPath(path, paint);

  canvas->Flush();
}

void WindowFrameDark::OnSetup() {
  CreateWidgets();
}

int WindowFrameDark::GetMouseLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->surface_x()), y = static_cast<int>(event->surface_y());

  // TODO: maximized or frameless

  if (x < (Theme::shadow_margin().left - kResizingMargin.left))
    hlocation = kExterior;
  else if (x < Theme::shadow_margin().left + kResizingMargin.left)
    hlocation = kResizeLeft;
  else if (x < Theme::shadow_margin().left + window()->geometry().width() - kResizingMargin.right)
    hlocation = kInterior;
  else if (x < Theme::shadow_margin().left + window()->geometry().width() + kResizingMargin.right)
    hlocation = kResizeRight;
  else
    hlocation = kExterior;

  if (y < (Theme::shadow_margin().top - kResizingMargin.top))
    vlocation = kExterior;
  else if (y < Theme::shadow_margin().top + kResizingMargin.top)
    vlocation = kResizeTop;
  else if (y < Theme::shadow_margin().top + window()->geometry().height() - kResizingMargin.bottom)
    vlocation = kInterior;
  else if (y < Theme::shadow_margin().top + window()->geometry().height() + kResizingMargin.bottom)
    vlocation = kResizeBottom;
  else
    vlocation = kExterior;

  location = vlocation | hlocation;
  if (location & kExterior)
    location = kExterior;

  if (location == kInterior &&
      y < Theme::shadow_margin().top + title_bar_size())
    location = kTitleBar;
  else if (location == kInterior)
    location = kClientArea;

  return location;
}

void WindowFrameDark::DrawShadow(Canvas *canvas) {

  const float rad = Theme::shadow_radius() - 1.f; // The spread radius

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

void *WindowFrameDarkCreate() {
  return new skland::WindowFrameDark;
}

void WindowFrameDarkDestroy(void *p) {
  skland::WindowFrameDark *frame = static_cast<skland::WindowFrameDark *>(p);
  delete frame;
}

