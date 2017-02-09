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

#include "theme-dark.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include <skland/gui/abstract-button.hpp>
#include <skland/gui/label.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-window.hpp>
#include <skland/gui/abstract-window-frame.hpp>
#include <skland/gui/context.hpp>

#include <skland/graphic/gradient-shader.hpp>

#include "SkCanvas.h"

namespace skland {

/**
 * @brief The default window frame
 */
class WindowFrameDark final : public AbstractWindowFrame {

  WindowFrameDark(const WindowFrameDark &orig) = delete;
  WindowFrameDark &operator=(const WindowFrameDark &other) = delete;

 public:

  WindowFrameDark();

  virtual ~WindowFrameDark();

 protected:

  virtual void OnSetup();

  virtual void OnResize(int width, int height);

  virtual void OnDraw(const Context *context);

  /**
    * Calculate and return the cursor position at the border of this frame
    * @param x
    * @param y
    * @return
    *
    * |        |                 |
    * |        |                 |
    * | shadow | resizing border | resizing border
    * |        |                 |
    * |        |                 |
    */
  virtual int GetMouseLocation(const MouseEvent *event) const;

 private:

/// @cond IGNORE

  class CloseButton final : public AbstractButton {

   public:

    CloseButton();

    virtual ~CloseButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(const Context *context) final;

  };

  class MaximizeButton final : public AbstractButton {

   public:

    MaximizeButton();

    virtual ~MaximizeButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(const Context *context) final;

  };

  class MinimizeButton final : public AbstractButton {

   public:

    MinimizeButton();

    virtual ~MinimizeButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(const Context *context) final;

  };

/// @endcond

  void OnCloseButtonClicked(__SLOT__);

  void OnMaximizeButtonClicked(__SLOT__);

  void OnMinimizeButtonClicked(__SLOT__);

  void CreateWidgets();

  void LayoutWidgets(int width, int height);

  void DrawShadow(Canvas *canvas);

  CloseButton *close_button_;

  MaximizeButton *maximize_button_;

  MinimizeButton *minimize_button_;

  Label *title_;

  static const int kButtonSize = 14;
  static const int kButtonSpace = 5;

};

WindowFrameDark::CloseButton::CloseButton()
    : AbstractButton() {
  resize(kButtonSize, kButtonSize);
}

WindowFrameDark::CloseButton::~CloseButton() {
}

Size WindowFrameDark::CloseButton::GetPreferredSize() const {
  return Size(kButtonSize, kButtonSize);
}

void WindowFrameDark::CloseButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameDark::CloseButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->GetCanvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Color regulars[2] = {0xFF999999, 0xFF666666};
  float pos[2] = {0.33f, 1.f};
  Color outline = regulars[0];

  if (IsHovered()) {
    if (IsPressed()) {
      regulars[0] = regulars[0] - 25;
      regulars[1] = regulars[1] - 25;
    } else {
      regulars[0] = regulars[0] + 15;
      regulars[1] = regulars[1] + 15;
    }
  }

  Shader shader =
      GradientShader::MakeRadial(Point2F(geometry().center_x(), geometry().center_y()),
                                 6.f, regulars, pos, 2, Shader::TileMode::kTileModeClamp);

  Paint paint;
  paint.SetShader(shader);
  paint.SetAntiAlias(true);

  paint.SetStyle(Paint::Style::kStyleFill);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.f, paint);

  paint.Reset();
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::Style::kStyleStroke);
  paint.SetColor(outline);
  paint.SetStrokeWidth(1.f);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 5.5f, paint);

  if (IsHovered()) {
    paint.SetColor(regulars[1]);
    paint.SetStrokeWidth(1.5f);
    canvas->DrawLine(geometry().center_x() - 2.5f, geometry().center_y() - 2.5f,
                     geometry().center_x() + 2.5f, geometry().center_y() + 2.5f,
                     paint);
    canvas->DrawLine(geometry().center_x() + 2.5f, geometry().center_y() - 2.5f,
                     geometry().center_x() - 2.5f, geometry().center_y() + 2.5f,
                     paint);
  }

  canvas->Restore();
}

WindowFrameDark::MaximizeButton::MaximizeButton()
    : AbstractButton() {
  resize(kButtonSize, kButtonSize);
}

WindowFrameDark::MaximizeButton::~MaximizeButton() {

}

Size WindowFrameDark::MaximizeButton::GetPreferredSize() const {
  return Size(kButtonSize, kButtonSize);
}

void WindowFrameDark::MaximizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameDark::MaximizeButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->GetCanvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Color regulars[2] = {0xFF999999, 0xFF666666};
  float pos[2] = {0.33f, 1.f};
  Color outline = regulars[0];

  if (IsHovered()) {
    if (IsPressed()) {
      regulars[0] = regulars[0] - 25;
      regulars[1] = regulars[1] - 25;
    } else {
      regulars[0] = regulars[0] + 15;
      regulars[1] = regulars[1] + 15;
    }
  }

  Shader shader =
      GradientShader::MakeRadial(Point2F(geometry().center_x(), geometry().center_y()),
                                 6.f, regulars, pos, 2, Shader::TileMode::kTileModeClamp);

  Paint paint;
  paint.SetShader(shader);
  paint.SetAntiAlias(true);

  paint.SetStyle(Paint::Style::kStyleFill);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.f, paint);

  paint.Reset();
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::Style::kStyleStroke);
  paint.SetColor(outline);
  paint.SetStrokeWidth(1.f);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 5.5f, paint);

  if (IsHovered()) {
    paint.SetColor(regulars[1]);
    paint.SetStrokeWidth(1.5f);
    canvas->DrawLine(geometry().center_x() - 3.25f, geometry().center_y(),
                     geometry().center_x() + 3.25f, geometry().center_y(),
                     paint);
    canvas->DrawLine(geometry().center_x(), geometry().center_y() - 3.25f,
                     geometry().center_x(), geometry().center_y() + 3.25f,
                     paint);
  }

  canvas->Restore();
}

WindowFrameDark::MinimizeButton::MinimizeButton()
    : AbstractButton() {
  resize(kButtonSize, kButtonSize);
}

WindowFrameDark::MinimizeButton::~MinimizeButton() {

}

Size WindowFrameDark::MinimizeButton::GetPreferredSize() const {
  return Size(kButtonSize, kButtonSize);
}

void WindowFrameDark::MinimizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameDark::MinimizeButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->GetCanvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Color regulars[2] = {0xFF999999, 0xFF666666};
  float pos[2] = {0.33f, 1.f};
  Color outline = regulars[0];

  if (IsHovered()) {
    if (IsPressed()) {
      regulars[0] = regulars[0] - 25;
      regulars[1] = regulars[1] - 25;
    } else {
      regulars[0] = regulars[0] + 15;
      regulars[1] = regulars[1] + 15;
    }
  }

  Shader shader =
      GradientShader::MakeRadial(Point2F(geometry().center_x(), geometry().center_y()),
                                 6.f, regulars, pos, 2, Shader::TileMode::kTileModeClamp);

  Paint paint;
  paint.SetShader(shader);
  paint.SetAntiAlias(true);

  paint.SetStyle(Paint::Style::kStyleFill);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 6.f, paint);

  paint.Reset();
  paint.SetAntiAlias(true);
  paint.SetStyle(Paint::Style::kStyleStroke);
  paint.SetColor(outline);
  paint.SetStrokeWidth(1.f);
  canvas->DrawCircle(geometry().center_x(), geometry().center_y(), 5.5f, paint);

  if (IsHovered()) {
    paint.SetColor(regulars[1]);
    paint.SetStrokeWidth(1.5f);
    canvas->DrawLine(geometry().center_x() - 3.25f, geometry().center_y(),
                     geometry().center_x() + 3.25f, geometry().center_y(),
                     paint);
  }

  canvas->Restore();
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
  delete maximize_button_;
  delete minimize_button_;
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

  minimize_button_ = new MinimizeButton;
  minimize_button_->clicked().Connect(this, &WindowFrameDark::OnMinimizeButtonClicked);

  maximize_button_ = new MaximizeButton;
  maximize_button_->clicked().Connect(this, &WindowFrameDark::OnMaximizeButtonClicked);

  title_ = new Label(window()->title());
  title_->SetForeground(0xFFBBBBBB);
  title_->SetFont(Font(Typeface::kBold));

  AddWidget(title_);  // put the title below other widgets
  AddWidget(close_button_);
  AddWidget(maximize_button_);
  AddWidget(minimize_button_);

  LayoutWidgets(window()->width(), window()->height());
}

void WindowFrameDark::OnResize(int width, int height) {
  LayoutWidgets(width, height);
}

void WindowFrameDark::LayoutWidgets(int width, int height) {
  title_->MoveTo(0, 0);
  title_->Resize(window()->width(), title_bar_size());

  int y = (title_bar_size() - kButtonSize) / 2;
  int x = kButtonSpace;
  close_button_->MoveTo(x, y);

  x += close_button_->width() + kButtonSpace;
  maximize_button_->MoveTo(x, y);

  x += maximize_button_->width() + kButtonSpace;
  minimize_button_->MoveTo(x, y);
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

  // Drop shadow:
  canvas->Save();
  canvas->ClipPath(path, kClipDifference, true);
  DrawShadow(canvas.get());
  canvas->Restore();

  // Fill color:
  Paint paint;
  paint.SetAntiAlias(true);
  paint.SetColor(0xEF202020);
  canvas->DrawPath(path, paint);

  // Draw the client area:
  paint.SetColor(0xEF303030);
  canvas->Save();
  canvas->ClipPath(path, kClipIntersect, true);
  canvas->DrawRect(GetClientGeometry(), paint);
  canvas->Restore();

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

