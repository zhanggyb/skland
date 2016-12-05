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

#include <skland/gui/abstract-window-frame.hpp>
#include <skland/gui/abstract-window.hpp>
#include <skland/gui/push-button.hpp>

namespace skland {

 const Margin AbstractWindowFrame::kShadowMargin(25, 35, 45, 35);
//const Margin AbstractWindowFrame::kShadowMargin(0, 0, 0, 0);

const Margin AbstractWindowFrame::kResizingMargin(5, 5, 5, 5);

const Point AbstractWindowFrame::kShadowOffset(0, 10);

const int AbstractWindowFrame::kShadowBlurRadius = 30;

AbstractWindowFrame::AbstractWindowFrame(AbstractWindow *window,
                                         int border,
                                         TitleBarPosition title_bar_position,
                                         int title_bar_size)
    : window_(window),
      border_(border),
      title_bar_size_(title_bar_size),
      title_bar_position_(title_bar_position) {
}

AbstractWindowFrame::~AbstractWindowFrame() {
}

Rect AbstractWindowFrame::GetLocalClientGeometry() const {
  int x = border_,
      y = border_,
      w = window()->width() - 2 * border_,
      h = window()->height() - 2 * border_;

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

void AbstractWindowFrame::AddWidget(AbstractWidget *widget, int pos) {
  window_->AddSubView(widget, pos);
}

}
