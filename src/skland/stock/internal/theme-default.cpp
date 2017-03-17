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

#include "../../gui/internal/abstract-view-redraw-task-iterator.hpp"

#include "SkCanvas.h"
//#include "SkTypeface.h"
//#include "SkPaint.h"
#include "SkTextBox.h"

namespace skland {

CloseButton::CloseButton()
    : AbstractButton(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize),
      foreground_(0xFF444444),
      background_(0xFF999999) {
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
    : AbstractButton(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize),
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
  return Size(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize);
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
    : AbstractButton(WindowFrameDefault::kButtonSize, WindowFrameDefault::kButtonSize),
      foreground_(0xFF444444),
      background_(0xFF999999) {
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

void TitleBar::OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {
  int new_y = (GetHeight() - WindowFrameDefault::kButtonSize) / 2;
  int new_x = WindowFrameDefault::kButtonSpace;
  close_button_->MoveTo(new_x, new_y);

  new_x += close_button_->GetWidth() + WindowFrameDefault::kButtonSpace;
  maximize_button_->MoveTo(new_x, new_y);

  new_x += maximize_button_->GetWidth() + WindowFrameDefault::kButtonSpace;
  minimize_button_->MoveTo(new_x, new_y);

  RecursiveUpdate();
}

void TitleBar::OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) {

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

// -------

WindowFrameDefault::WindowFrameDefault()
    : AbstractShellFrame(),
      border_(0),
      title_bar_size_(22),
      title_bar_position_(kTop),
      title_bar_(nullptr) {
  title_bar_ = new TitleBar;
  SetTitleView(title_bar_);
}

WindowFrameDefault::~WindowFrameDefault() {

}

Rect WindowFrameDefault::GetClientGeometry(int width, int height) const {
  int x = border_,
      y = border_,
      w = width - 2 * border_,
      h = height - 2 * border_;

  switch (title_bar_position_) {
    case kLeft: {
      x += title_bar_size_ - border_;
      break;
    }
    case kRight: {
      w -= title_bar_size_ + border_;
      break;
    }
    case kBottom: {
      h -= title_bar_size_ + border_;
      break;
    }
    case kTop:
    default: {
      y += title_bar_size_ - border_;
      h -= title_bar_size_ - border_;
      break;
    }
  }

  return Rect::FromXYWH(x, y, w, h);
}

void WindowFrameDefault::OnCloseButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractShellView::kClose);
}

void WindowFrameDefault::OnMaximizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractShellView::kMaximize);
}

void WindowFrameDefault::OnMinimizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractShellView::kMinimize);
}

void WindowFrameDefault::OnSetup() {
  title_bar_->close_button()->clicked().Connect(this, &WindowFrameDefault::OnCloseButtonClicked);
  title_bar_->maximize_button()->clicked().Connect(this, &WindowFrameDefault::OnMaximizeButtonClicked);
  title_bar_->minimize_button()->clicked().Connect(this, &WindowFrameDefault::OnMinimizeButtonClicked);
  title_bar_->SetTitle(GetShellView()->GetTitle());
}

void WindowFrameDefault::OnResize(int width, int height) {
  title_bar_->MoveTo(0, 0);
  title_bar_->Resize(width, title_bar_size_);
}

void WindowFrameDefault::OnDraw(const Context *context) {
  AbstractShellView *shell_view = GetShellView();

  std::shared_ptr<Canvas> canvas = context->canvas();
  canvas->Clear();

  Path path;
  Rect geometry = Rect::FromXYWH(0.f, 0.f,
                                 shell_view->GetSize().width,
                                 shell_view->GetSize().height);

  // Drop shadow:
  if ((!shell_view->IsMaximized()) || (!shell_view->IsFullscreen())) {
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
  canvas->DrawRect(GetClientGeometry(GetShellView()->GetSize().width, GetShellView()->GetSize().height), paint);
  canvas->Restore();

  canvas->Flush();
}

int WindowFrameDefault::GetMouseLocation(const MouseEvent *event) const {
  int vlocation, hlocation, location;
  int x = static_cast<int>(event->GetSurfaceXY().x),
      y = static_cast<int>(event->GetSurfaceXY().y);

  // TODO: maximized or frameless

  if (x < (Theme::shadow_margin().left - kResizingMargin.left))
    hlocation = AbstractShellView::kExterior;
  else if (x < Theme::shadow_margin().left + kResizingMargin.left)
    hlocation = AbstractShellView::kResizeLeft;
  else if (x < Theme::shadow_margin().left + GetShellView()->GetSize().width - kResizingMargin.right)
    hlocation = AbstractShellView::kInterior;
  else if (x < Theme::shadow_margin().left + GetShellView()->GetSize().width + kResizingMargin.right)
    hlocation = AbstractShellView::kResizeRight;
  else
    hlocation = AbstractShellView::kExterior;

  if (y < (Theme::shadow_margin().top - kResizingMargin.top))
    vlocation = AbstractShellView::kExterior;
  else if (y < Theme::shadow_margin().top + kResizingMargin.top)
    vlocation = AbstractShellView::kResizeTop;
  else if (y < Theme::shadow_margin().top + GetShellView()->GetSize().height - kResizingMargin.bottom)
    vlocation = AbstractShellView::kInterior;
  else if (y < Theme::shadow_margin().top + GetShellView()->GetSize().height + kResizingMargin.bottom)
    vlocation = AbstractShellView::kResizeBottom;
  else
    vlocation = AbstractShellView::kExterior;

  location = vlocation | hlocation;
  if (location & AbstractShellView::kExterior)
    location = AbstractShellView::kExterior;

  if (location == AbstractShellView::kInterior &&
      y < Theme::shadow_margin().top + title_bar_size_)
    location = AbstractShellView::kTitleBar;
  else if (location == AbstractShellView::kInterior)
    location = AbstractShellView::kClientArea;

  return location;
}

void WindowFrameDefault::DrawShadow(Canvas *canvas) {
  const Size &size = GetShellView()->GetSize();
  float rad = Theme::shadow_radius() - 1.f; // The spread radius
  float offset_x = Theme::shadow_offset_x();
  float offset_y = Theme::shadow_offset_y();

  if (!GetShellView()->IsFocused()) {
    rad = (int) rad / 3;
    offset_x = (int) offset_x / 3;
    offset_y = (int) offset_y / 3;
  }

  // shadow map
  SkCanvas *c = canvas->GetSkCanvas();
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
                                    size.width - 2 * rad, 2 * rad),
                   nullptr);

  // top-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 0,
                                    250, 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(size.width - rad + offset_x, -rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(-rad + offset_x, rad + offset_y,
                                    2 * rad, size.height - 2 * rad),
                   nullptr);

  // bottom-left
  c->drawImageRect(image,
                   SkRect::MakeLTRB(0, 250 - 2 * Theme::shadow_radius(),
                                    2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(-rad + offset_x, size.height - rad + offset_y,
                                    2 * rad, 2 * rad),
                   nullptr);

  // bottom
  c->drawImageRect(image,
                   SkRect::MakeLTRB(2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250 - 2 * Theme::shadow_radius(), 250),
                   SkRect::MakeXYWH(rad + offset_x, size.height - rad + offset_y,
                                    size.width - 2 * rad, 2 * rad),
                   nullptr);

  // bottom-right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 250 - 2 * Theme::shadow_radius(),
                                    250, 250),
                   SkRect::MakeXYWH(size.width - rad + offset_x,
                                    size.height - rad + offset_y,
                                    2 * rad,
                                    2 * rad),
                   nullptr);

  // right
  c->drawImageRect(image,
                   SkRect::MakeLTRB(250 - 2 * Theme::shadow_radius(), 2 * Theme::shadow_radius(),
                                    250, 250 - 2 * Theme::shadow_radius()),
                   SkRect::MakeXYWH(size.width - rad + offset_x, rad + offset_y,
                                    2 * rad, size.height - 2 * rad),
                   nullptr);

}

}
