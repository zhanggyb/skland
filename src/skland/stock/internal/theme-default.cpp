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

#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/abstract-shell-view.hpp>
#include <skland/gui/context.hpp>

#include <skland/stock/theme.hpp>

#include "../../gui/internal/abstract-event-handler-redraw-task-iterator.hpp"

#include "SkCanvas.h"
//#include "SkTypeface.h"
//#include "SkPaint.h"
#include "SkTextBox.h"

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

Size TitleBar::GetMinimalSize() const {
  return Size(0, 0);
}

Size TitleBar::GetPreferredSize() const {
  return Size(240, 22);
}

Size TitleBar::GetMaximalSize() const {
  return Size(65536, 65536);
}

void TitleBar::SetForeground(const Color &color) {
  foreground_ = color;
  Update();
}

void TitleBar::SetTitle(const std::string &title) {
  title_ = title;
  Update();
}

void TitleBar::OnUpdate(AbstractView *view) {
  RedrawTaskIterator it(this);

  if (it.IsLinked()) {
    DBG_ASSERT(it.context().canvas());
    if (view == this) return;

    AbstractView::OnUpdate(close_button_);
    AbstractView::OnUpdate(maximize_button_);
    AbstractView::OnUpdate(minimize_button_);
  } else {
    AbstractView::OnUpdate(view);

    if (view == this && it.IsLinked()) {
      DBG_ASSERT(it.context().canvas());
      AbstractView::OnUpdate(close_button_);
      AbstractView::OnUpdate(maximize_button_);
      AbstractView::OnUpdate(minimize_button_);
    }
  }
}

void TitleBar::OnPositionChanged(int x, int y) {
  Update();

  y = (height() - WindowFrameDefault::kButtonSize) / 2;
  x = WindowFrameDefault::kButtonSpace;
  close_button_->MoveTo(x, y);
//  close_button_->Update();

  x += close_button_->width() + WindowFrameDefault::kButtonSpace;
  maximize_button_->MoveTo(x, y);
//  maximize_button_->Update();

  x += maximize_button_->width() + WindowFrameDefault::kButtonSpace;
  minimize_button_->MoveTo(x, y);
//  minimize_button_->Update();
}

void TitleBar::OnSizeChanged(int width, int height) {
  Update();

  int y = (height - WindowFrameDefault::kButtonSize) / 2;
  int x = WindowFrameDefault::kButtonSpace;
  close_button_->MoveTo(x, y);
  close_button_->Update();

  x += close_button_->width() + WindowFrameDefault::kButtonSpace;
  maximize_button_->MoveTo(x, y);
  maximize_button_->Update();

  x += maximize_button_->width() + WindowFrameDefault::kButtonSpace;
  minimize_button_->MoveTo(x, y);
  minimize_button_->Update();
}

void TitleBar::OnMouseEnter(MouseEvent *event) {
  event->Ignore();
}

void TitleBar::OnMouseLeave(MouseEvent *event) {
  event->Ignore();
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
  // Put the text at the center
  text_box.setBox((geometry().l + geometry().r - text_width) / 2.f,
                  geometry().t + 1.f, // move down a little for better look
                  (geometry().r - geometry().l + text_width) / 2.f,
                  geometry().b);
  text_box.setSpacingAlign(SkTextBox::kCenter_SpacingAlign);
  text_box.setText(title_.c_str(), title_.length(), *paint.sk_paint());
  text_box.draw(context->canvas()->sk_canvas());
}

// -------

WindowFrameDefault::WindowFrameDefault()
    : AbstractShellFrame(),
      border_(0),
      title_bar_size_(22),
      title_bar_position_(kTitleBarTop),
      title_bar_(nullptr) {
  title_bar_ = new TitleBar;
  SetTitleBar(title_bar_);
}

WindowFrameDefault::~WindowFrameDefault() {

}

Rect WindowFrameDefault::GetContentGeometry() const {
  int x = border_,
      y = border_,
      w = shell_view()->GetSize().width - 2 * border_,
      h = shell_view()->GetSize().height - 2 * border_;

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

void WindowFrameDefault::OnCloseButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractShellView::kActionClose);
}

void WindowFrameDefault::OnMaximizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractShellView::kActionMaximize);
}

void WindowFrameDefault::OnMinimizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractShellView::kActionMinimize);
}

void WindowFrameDefault::OnSetup() {
  title_bar_->close_button()->clicked().Connect(this, &WindowFrameDefault::OnCloseButtonClicked);
  title_bar_->maximize_button()->clicked().Connect(this, &WindowFrameDefault::OnMaximizeButtonClicked);
  title_bar_->minimize_button()->clicked().Connect(this, &WindowFrameDefault::OnMinimizeButtonClicked);
  title_bar_->SetTitle(shell_view()->GetTitle());
}

void WindowFrameDefault::OnResize(int width, int height) {
  title_bar_->MoveTo(0, 0);
  title_bar_->Resize(shell_view()->GetSize().width, title_bar_size_);
}

void WindowFrameDefault::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Clear();

  Path path;
  Rect geometry = Rect::FromXYWH(0.f, 0.f, shell_view()->GetSize().width, shell_view()->GetSize().height);

  // Drop shadow:
  if ((!shell_view()->IsMaximized()) || (!shell_view()->IsFullscreen())) {
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
  canvas->DrawRect(GetContentGeometry(), paint);
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
  else if (x < Theme::shadow_margin().left + shell_view()->GetSize().width - kResizingMargin.right)
    hlocation = kInterior;
  else if (x < Theme::shadow_margin().left + shell_view()->GetSize().width + kResizingMargin.right)
    hlocation = kResizeRight;
  else
    hlocation = kExterior;

  if (y < (Theme::shadow_margin().top - kResizingMargin.top))
    vlocation = kExterior;
  else if (y < Theme::shadow_margin().top + kResizingMargin.top)
    vlocation = kResizeTop;
  else if (y < Theme::shadow_margin().top + shell_view()->GetSize().height - kResizingMargin.bottom)
    vlocation = kInterior;
  else if (y < Theme::shadow_margin().top + shell_view()->GetSize().height + kResizingMargin.bottom)
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

  if (!shell_view()->IsFocused()) {
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
                                    shell_view()->GetSize().width - 2 * rad, 2 * rad),
                   nullptr);

  // top-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 0,
                                    250, 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(shell_view()->GetSize().width - rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(-rad + offset_x, rad + offset_y,
                                    2 * rad, shell_view()->GetSize().height - 2 * rad),
                   nullptr);

  // bottom-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 250 - 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(-rad + offset_x, shell_view()->GetSize().height - rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // bottom
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250 - 2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(rad + offset_x, shell_view()->GetSize().height - rad + offset_y,
                                    shell_view()->GetSize().width - 2 * rad, 2 * rad),
                   nullptr);

  // bottom-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250, 250),
                   SkRect::MakeXYWH(shell_view()->GetSize().width - rad + offset_x,
                                    shell_view()->GetSize().height - rad + offset_y,
                                    2 * rad,
                                    2 * rad),
                   nullptr);

  // right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 2 * Theme::shadow_radius(),
                                    250, 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(shell_view()->GetSize().width - rad + offset_x, rad + offset_y,
                                    2 * rad, shell_view()->GetSize().height - 2 * rad),
                   nullptr);

}

}
