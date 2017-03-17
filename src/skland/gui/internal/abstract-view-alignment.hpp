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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_ALIGNMENT_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_ALIGNMENT_HPP_

#include <skland/gui/abstract-view.hpp>
#include <skland/core/types.hpp>

namespace skland {

/**
 * @ingroup gui_intern
 * @brief Used to align views in layout
 */
SKLAND_NO_EXPORT struct AbstractView::Alignment {

  Alignment() = delete;
  Alignment(const Alignment &) = delete;
  Alignment &operator=(const Alignment &) = delete;

  Alignment(AbstractView *view)
      : view(view),
        base(nullptr),
        align(kAlignUndefined),
        distance(0),
        contrary(nullptr),
        previous(nullptr),
        next(nullptr) {}

  ~Alignment() {}

  AbstractView *view;

  AbstractView *base;
  Align align;
  int distance;

  Alignment *contrary;

  Alignment *previous;
  Alignment *next;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_ALIGNMENT_HPP_
