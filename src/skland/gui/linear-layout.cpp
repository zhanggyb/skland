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

#include <skland/gui/linear-layout.hpp>

#include "internal/abstract-view-iterators.hpp"

namespace skland {

LinearLayout::LinearLayout(Orientation orientation, const Padding &padding, int space)
    : AbstractLayout(padding) {

}

LinearLayout::~LinearLayout() {

}

Size LinearLayout::GetPreferredSize() const {
  Size size(padding().lr(), padding().tb());
  Size tmp;
  int space = 0;
  ConstIterator it(this);

  if (orientation_ == kVertical) {
    for (it = it.first_child(); it; ++it) {
      if (it.view()->IsVisible()) {
        tmp = it.view()->GetPreferredSize();
        size.width = std::max(size.width, tmp.width);
        size.height += (tmp.height + space);
        space += space_;
      }
    }
  } else {
    for (it = it.first_child(); it; ++it) {
      if (it.view()->IsVisible()) {
        tmp = it.view()->GetPreferredSize();
        size.width += (tmp.width + space);
        size.height = std::max(size.height, tmp.height);
        space += space_;
      }
    }
  }

  return size;
}

void LinearLayout::OnMeasureReposition(int x, int y) {
  Update();
}

void LinearLayout::OnMeasureResize(int width, int height) {
  Update();
}

void LinearLayout::OnGeometryChanged(int flag, const Rect &old_geometry, const Rect &new_geometry) {

}

void LinearLayout::OnViewAdded(AbstractView *view) {
  // TODO:
}

void LinearLayout::OnViewRemoved(AbstractView *view) {
  // TODO:
}

}
