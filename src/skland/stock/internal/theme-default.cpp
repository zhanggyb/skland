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
#include <skland/gui/title-bar.hpp>

#include <skland/stock/theme.hpp>

#include "SkCanvas.h"
//#include "SkTypeface.h"
//#include "SkPaint.h"
#include "SkTextBox.h"

namespace skland {

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
//  title_bar_->close_button()->clicked().Connect(this, &WindowFrameDefault::OnCloseButtonClicked);
//  title_bar_->maximize_button()->clicked().Connect(this, &WindowFrameDefault::OnMaximizeButtonClicked);
//  title_bar_->minimize_button()->clicked().Connect(this, &WindowFrameDefault::OnMinimizeButtonClicked);
//  title_bar_->SetTitle(GetShellView()->GetTitle());
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
                                 shell_view->GetWidth(),
                                 shell_view->GetHeight());

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
  canvas->DrawRect(GetClientGeometry(GetShellView()->GetWidth(), GetShellView()->GetHeight()), paint);
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
  else if (x < Theme::shadow_margin().left + GetShellView()->GetWidth() - kResizingMargin.right)
    hlocation = AbstractShellView::kInterior;
  else if (x < Theme::shadow_margin().left + GetShellView()->GetWidth() + kResizingMargin.right)
    hlocation = AbstractShellView::kResizeRight;
  else
    hlocation = AbstractShellView::kExterior;

  if (y < (Theme::shadow_margin().top - kResizingMargin.top))
    vlocation = AbstractShellView::kExterior;
  else if (y < Theme::shadow_margin().top + kResizingMargin.top)
    vlocation = AbstractShellView::kResizeTop;
  else if (y < Theme::shadow_margin().top + GetShellView()->GetHeight() - kResizingMargin.bottom)
    vlocation = AbstractShellView::kInterior;
  else if (y < Theme::shadow_margin().top + GetShellView()->GetHeight() + kResizingMargin.bottom)
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
  const Size size(GetShellView()->GetWidth(), GetShellView()->GetHeight());
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
