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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_

#include <skland/gui/abstract-view.hpp>

#include "redraw-task.hpp"
#include "view-task.hpp"

namespace skland {

struct AbstractView::Private {

  Private() = delete;
  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private(AbstractView *view)
      : redraw_task(view),
        mouse_task(view),
        mouse_motion_task(view),
        is_damaged_(false) {}

  ~Private() {}

  RedrawTask redraw_task;
  ViewTask mouse_task;
  ViewTask mouse_motion_task;

  // Damage area
  bool is_damaged_;
  RectI damaged_region_;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
