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

#ifndef SKLAND_GUI_LINEAR_LAYOUT_HPP_
#define SKLAND_GUI_LINEAR_LAYOUT_HPP_

#include <skland/core/types.hpp>
#include "abstract-layout.hpp"

namespace skland {

SKLAND_EXPORT class LinearLayout final : public AbstractLayout {

  LinearLayout(const LinearLayout &) = delete;
  LinearLayout &operator=(const LinearLayout &) = delete;

 public:

  LinearLayout(Orientation orientation = kHorizontal,
               const Padding &padding = Padding(5),
               int space = 5);

  virtual ~LinearLayout();

  virtual Size GetPreferredSize() const;

 protected:

  virtual void OnMeasureReposition(int old_x, int old_y, int new_x, int new_y);

  virtual void OnMeasureResize(int old_width, int old_height, int new_width, int new_height);

  virtual void OnViewAdded(AbstractView *view);

  virtual void OnViewRemoved(AbstractView *view);

 private:

  Orientation orientation_;

  int space_;

};

}

#endif // SKLAND_GUI_LINEAR_LAYOUT_HPP_
