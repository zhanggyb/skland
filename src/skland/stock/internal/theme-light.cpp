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

#include "theme-light.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>

#include <skland/gui/abstract-button.hpp>
#include <skland/gui/label.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/abstract-window.hpp>

#include <skland/gui/context.hpp>
#include "SkCanvas.h"

namespace skland {

/**
 * @brief The default window frame
 */
class WindowFrameLight final : public AbstractWindowFrame {

  WindowFrameLight(const WindowFrameLight &orig) = delete;
  WindowFrameLight &operator=(const WindowFrameLight &other) = delete;

 public:

  WindowFrameLight();

  virtual ~WindowFrameLight();

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

};

WindowFrameLight::CloseButton::CloseButton()
    : AbstractButton() {
  resize(14, 14);
}

WindowFrameLight::CloseButton::~CloseButton() {
}

Size WindowFrameLight::CloseButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrameLight::CloseButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameLight::CloseButton::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->GetCanvas();
  canvas->Save();
  canvas->ClipRect(geometry());
  canvas->Clear();

  Color regular(0.95f, 0.55f, 0.f);
  // Color outline(0.78f, 0.28f, 0.28f);
  Color outline(0xFF444444);
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

WindowFrameLight::MaximizeButton::MaximizeButton()
    : AbstractButton() {
  resize(14, 14);
}

WindowFrameLight::MaximizeButton::~MaximizeButton() {

}

Size WindowFrameLight::MaximizeButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrameLight::MaximizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameLight::MaximizeButton::OnDraw(const Context *context) {
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

WindowFrameLight::MinimizeButton::MinimizeButton()
    : AbstractButton() {
  resize(14, 14);
}

WindowFrameLight::MinimizeButton::~MinimizeButton() {

}

Size WindowFrameLight::MinimizeButton::GetPreferredSize() const {
  return Size(14, 14);
}

void WindowFrameLight::MinimizeButton::OnResize(int /* width */, int /* height */) {

}

void WindowFrameLight::MinimizeButton::OnDraw(const Context *context) {
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

WindowFrameLight::WindowFrameLight()
    : AbstractWindowFrame(),
      close_button_(nullptr),
      maximize_button_(nullptr),
      minimize_button_(nullptr),
      title_(nullptr) {
  set_border(0);
  set_title_bar_size(22);
  set_title_bar_position(kTitleBarTop);
}

WindowFrameLight::~WindowFrameLight() {
  delete title_;
  delete close_button_;
}

void WindowFrameLight::OnCloseButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionClose);
}

void WindowFrameLight::OnMaximizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMaximize);
}

void WindowFrameLight::OnMinimizeButtonClicked(SLOT /* slot */) {
  EmitAction(AbstractWindow::kActionMinimize);
}

void WindowFrameLight::CreateWidgets() {
  close_button_ = new CloseButton;
  close_button_->clicked().Connect(this, &WindowFrameLight::OnCloseButtonClicked);

//  minimize_button_ = new MinimizeButton;
//  minimize_button_->clicked().Connect(this, &DefaultWindowFrame::OnMinimizeButtonClicked);

//  maximize_button_ = new MaximizeButton;
//  maximize_button_->clicked().Connect(this, &DefaultWindowFrame::OnMaximizeButtonClicked);

  title_ = new Label(window()->title());
  title_->SetForeground(0xFF444444);
  title_->SetFont(Font(Typeface::kBold));

  AddWidget(title_);  // put the title below other widgets
  AddWidget(close_button_);
//  AddWidget(maximize_button_);
//  AddWidget(minimize_button_);

  LayoutWidgets(window()->width(), window()->height());
}

void WindowFrameLight::OnResize(int width, int height) {
  LayoutWidgets(width, height);
}

void WindowFrameLight::LayoutWidgets(int width, int height) {
  title_->MoveTo(0, 0);
  title_->Resize(window()->width(), title_bar_size());

  int y = (title_bar_size() - (int) close_button_->geometry().height()) / 2;
  int x = y + 1;
  close_button_->MoveTo(x, y);
}

void WindowFrameLight::OnDraw(const Context *context) {
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

void WindowFrameLight::OnSetup() {
  CreateWidgets();
}

int WindowFrameLight::GetMouseLocation(const MouseEvent *event) const {
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

void WindowFrameLight::DrawShadow(Canvas *canvas) {

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

void *WindowFrameLightCreate() {
  return new skland::WindowFrameLight;
}

void WindowFrameLightDestroy(void *p) {
  skland::WindowFrameLight *frame = static_cast<skland::WindowFrameLight *>(p);
  delete frame;
}

