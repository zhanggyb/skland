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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_CONSTRAINT_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_CONSTRAINT_HPP_

#include <skland/gui/abstract-view.hpp>

#include <utility>

namespace skland {

/**
 * @ingroup gui_intern
 * @brief Used to align views in layout
 */
struct AbstractView::Anchor {

  Anchor() = delete;
  Anchor(const Anchor &) = delete;
  Anchor &operator=(const Anchor &) = delete;

  ~Anchor();

  Anchor *contrary;

  Anchor *previous;
  Anchor *next;
  AnchorGroup *group;

  std::shared_ptr<int> distance;

  static std::pair<Anchor *, Anchor *> MakePair(int distance,
                                                AbstractView *view1,
                                                AbstractView *view2);

 private:

  Anchor(AbstractView *view);

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_ALIGNMENT_HPP_
