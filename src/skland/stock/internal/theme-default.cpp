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

#include "theme-default.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include <skland/gui/label.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-window.hpp>
#include <skland/gui/context.hpp>
#include <skland/gui/linear-layout.hpp>

#include "SkCanvas.h"

namespace skland {

CloseButton::CloseButton()
    : AbstractButton(),
      foreground_(0xFF444444),
      background_(0xFF999999) {
  resize(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize);
}

CloseButton::~CloseButton() {
}

Size CloseButton::GetPreferredSize() const {
  return Size(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize);
}

void CloseButton::SetForeground(const Color &color) {
  foreground_ = color;
  Update();
}

void CloseButton::SetBackground(const Color &color) {
  background_ = color;
  Update();
}

void CloseButton::OnSizeChanged(int /* width */, int /* height */) {

}

void CloseButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(background_);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(foreground_);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.5f, paint);
  }

  paint.SetColor(foreground_);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(geometry().center_x() - 3.f, geometry().center_y() - 3.f,
                   geometry().center_x() + 3.f, geometry().center_y() + 3.f,
                   paint);
  canvas->DrawLine(geometry().center_x() + 3.f, geometry().center_y() - 3.f,
                   geometry().center_x() - 3.f, geometry().center_y() + 3.f,
                   paint);

  canvas->Restore();
}

MaximizeButton::MaximizeButton()
    : AbstractButton(),
      foreground_(0xFF444444),
      background_(0xFF999999) {
  resize(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize);
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
  return Size(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize);
}

void MaximizeButton::OnSizeChanged(int /* width */, int /* height */) {

}

void MaximizeButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(background_);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(foreground_);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.5f, paint);
  }

  paint.SetColor(foreground_);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(geometry().center_x() - 4.f, geometry().center_y(),
                   geometry().center_x() + 4.f, geometry().center_y(),
                   paint);
  canvas->DrawLine(geometry().center_x(), geometry().center_y() - 4.f,
                   geometry().center_x(), geometry().center_y() + 4.f,
                   paint);

  canvas->Restore();
}

MinimizeButton::MinimizeButton()
    : AbstractButton(),
      foreground_(0xFF444444),
      background_(0xFF999999) {
  resize(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize);
}

MinimizeButton::~MinimizeButton() {

}

Size MinimizeButton::GetPreferredSize() const {
  return Size(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize);
}

void MinimizeButton::SetForeground(const Color &color) {
  foreground_ = color;
  Update();
}

void MinimizeButton::SetBackground(const Color &color) {
  background_ = color;
  Update();
}

void MinimizeButton::OnSizeChanged(int /* width */, int /* height */) {

}

void MinimizeButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Paint paint;
  paint.SetAntiAlias(true);

  if (IsHovered()) {
    if (IsPressed()) {
      paint.SetColor(background_);
      paint.SetStyle(Paint::Style::kStyleFill);
      canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 7.f, paint);
      paint.Reset();
      paint.SetAntiAlias(true);
    }

    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetColor(foreground_);
    paint.SetStrokeWidth(1.f);
    canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.5f, paint);
  }

  paint.SetColor(foreground_);
  paint.SetStrokeWidth(1.5f);
  canvas->DrawLine(geometry().center_x() - 4.f, geometry().center_y(),
                   geometry().center_x() + 4.f, geometry().center_y(),
                   paint);

  canvas->Restore();
}

// -------

WindowFrameDefault::WindowFrameDefault()
    : AbstractWindowFrame(),
      border_(0),
      title_bar_size_(22),
      title_bar_position_(kTitleBarTop),
      close_button_(nullptr),
      maximize_button_(nullptr),
      minimize_button_(nullptr),
      title_(nullptr),
      layout_(nullptr) {
  CreateWidgets();
}

WindowFrameDefault::~WindowFrameDefault() {
  delete title_;
  delete maximize_button_;
  delete minimize_button_;
  delete close_button_;
  delete layout_;
}

Rect WindowFrameDefault::GetClientGeometry() const {
  int x = border_,
      y = border_,
      w = window()->size().width - 2 * border_,
      h = window()->size().height - 2 * border_;

  switch (title_bar_position_) {
    case kTitleBarLeft: {
      x += title_bar_size_ - border_;
      break;
    }
    case kTitleBarRight: {
      w -= title_bar_size_ + border_;
      break;
    }
    case kTitleBarBottom: {
      h -= title_bar_size_ + border_;
      break;
    }
    case kTitleBarTop:
    default: {
      y += title_bar_size_ - border_;
      h -= title_bar_size_ - border_;
      break;
    }
  }

  return Rect::FromXYWH(x, y, w, h);
}

AbstractView *WindowFrameDefault::GetContainer() const {
  return title_;
}

void WindowFrameDefault::OnCloseButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionClose);
}

void WindowFrameDefault::OnMaximizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMaximize);
}

void WindowFrameDefault::OnMinimizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMinimize);
}

void WindowFrameDefault::CreateWidgets() {
//  close_button_ = new CloseButton;
//  close_button_->clicked().Connect(this, &WindowFrameDefault::OnCloseButtonClicked);
//
//  minimize_button_ = new MinimizeButton;
//  minimize_button_->clicked().Connect(this, &WindowFrameDefault::OnMinimizeButtonClicked);
//
//  maximize_button_ = new MaximizeButton;
//  maximize_button_->clicked().Connect(this, &WindowFrameDefault::OnMaximizeButtonClicked);

//  title_ = new Label(window()->title());
  title_ = new Label("Test");
  title_->SetForeground(0xFF444444);
  title_->SetFont(Font(Typeface::kBold));

//  layout_ = new LinearLayout;

//  AddWidget(title_);  // put the title below other widgets
//  AddWidget(close_button_);
//  AddWidget(maximize_button_);
//  AddWidget(minimize_button_);

//  LayoutWidgets(window()->size().width, window()->height());

//  layout_->AddView(close_button_);
//  layout_->AddView(maximize_button_);
//  layout_->AddView(minimize_button_);
//  layout_->AddView(title_);

}

void WindowFrameDefault::OnResize(int width, int height) {
  LayoutWidgets(width, height);
}

void WindowFrameDefault::LayoutWidgets(int width, int height) {
  title_->MoveTo(0, 0);
  title_->Resize(window()->size().width, title_bar_size_);

//  title_->MoveTo(0, 0);
//  title_->Resize(window()->size().width, title_bar_size_);
//
//  int y = (title_bar_size_ - kButtonSize) / 2;
//  int x = kButtonSpace;
//  close_button_->MoveTo(x, y);
//
//  x += close_button_->width() + kButtonSpace;
//  maximize_button_->MoveTo(x, y);
//
//  x += maximize_button_->width() + kButtonSpace;
//  minimize_button_->MoveTo(x, y);
}

void WindowFrameDefault::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Clear();

  Path path;
  Rect geometry = Rect::FromXYWH(0.f, 0.f, window()->size().width, window()->size().height);

  // Drop shadow:
  if ((!window()->IsMaximized()) || (!window()->IsFullscreen())) {
    float radii[] = {
        7.f, 7.f, // top-left
        7.f, 7.f, // top-right
        4.f, 4.f, // bottom-right
        4.f, 4.f  // bottom-left
    };
    path.AddRoundRect(geometry, radii);
    canvas->Save();
    canvas->ClipPath(path, kClipDifference, true);
    DrawShadow(canvas.get());
    canvas->Restore();
  } else {
    path.AddRect(geometry);
  }

  // Fill color:
  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(0xEFF0F0F0);
  canvas->DrawPath(path, paint);

  // Draw the client area:
  paint.SetColor(0xEFE0E0E0);
  canvas->Save();
  canvas->ClipPath(path, kClipIntersect, true);
  canvas->DrawRect(GetClientGeometry(), paint);
  canvas->Restore();

  canvas->Flush();
}

int WindowFrameDefault::GetMouseLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->surface_x()), y = static_cast<int>(event->surface_y());

  // TODO: maximized or frameless

  if (x < (Theme::shadow_margin().left - kResizingMargin.left))
    hlocation = kExterior;
  else if (x < Theme::shadow_margin().left + kResizingMargin.left)
    hlocation = kResizeLeft;
  else if (x < Theme::shadow_margin().left + window()->size().width - kResizingMargin.right)
    hlocation = kInterior;
  else if (x < Theme::shadow_margin().left + window()->size().width + kResizingMargin.right)
    hlocation = kResizeRight;
  else
    hlocation = kExterior;

  if (y < (Theme::shadow_margin().top - kResizingMargin.top))
    vlocation = kExterior;
  else if (y < Theme::shadow_margin().top + kResizingMargin.top)
    vlocation = kResizeTop;
  else if (y < Theme::shadow_margin().top + window()->size().height - kResizingMargin.bottom)
    vlocation = kInterior;
  else if (y < Theme::shadow_margin().top + window()->size().height + kResizingMargin.bottom)
    vlocation = kResizeBottom;
  else
    vlocation = kExterior;

  location = vlocation | hlocation;
  if (location & kExterior)
    location = kExterior;

  if (location == kInterior &&
      y < Theme::shadow_margin().top + title_bar_size_)
    location = kTitleBar;
  else if (location == kInterior)
    location = kClientArea;

  return location;
}

void WindowFrameDefault::DrawShadow(Canvas *canvas) {
  float rad = Theme::shadow_radius() - 1.f; // The spread radius
  float offset_x = Theme::shadow_offset_x();
  float offset_y = Theme::shadow_offset_y();

  if (!window()->IsFocused()) {
    rad = (int) rad / 3;
    offset_x = (int) offset_x / 3;
    offset_y = (int) offset_y / 3;
  }

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
                                    window()->size().width - 2 * rad, 2 * rad),
                   nullptr);

  // top-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 0,
                                    250, 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(window()->size().width - rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(-rad + offset_x, rad + offset_y,
                                    2 * rad, window()->size().height - 2 * rad),
                   nullptr);

  // bottom-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 250 - 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(-rad + offset_x, window()->size().height - rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // bottom
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250 - 2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(rad + offset_x, window()->size().height - rad + offset_y,
                                    window()->size().width - 2 * rad, 2 * rad),
                   nullptr);

  // bottom-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250, 250),
                   SkRect::MakeXYWH(window()->size().width - rad + offset_x,
                                    window()->size().height - rad + offset_y,
                                    2 * rad,
                                    2 * rad),
                   nullptr);

  // right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 2 * Theme::shadow_radius(),
                                    250, 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(window()->size().width - rad + offset_x, rad + offset_y,
                                    2 * rad, window()->size().height - 2 * rad),
                   nullptr);

}

}
