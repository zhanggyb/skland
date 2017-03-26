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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_CONSTRAINT_GROUP_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_CONSTRAINT_GROUP_HPP_

#include <skland/gui/abstract-view.hpp>
#include <skland/core/types.hpp>
#include <skland/core/deque.hpp>

namespace skland {

/**
 * @ingroup gui_intern
 * @brief Anchor group
 */
class AbstractView::AnchorGroup {

  AnchorGroup() = delete;
  AnchorGroup(const AnchorGroup &) = delete;
  AnchorGroup &operator=(const AnchorGroup &) = delete;

 public:

  AnchorGroup(AbstractView *view, Alignment align);

  ~AnchorGroup();

  /**
   * @brief Push front a new created anchor object
   * @param anchor
   */
  void PushFront(Anchor *anchor);

  /**
   * @brief Insert a new created anchor object at the given position
   * @param index
   * @param anchor
   */
  void Insert(Anchor *anchor, int index);

  /**
   * @brief Push back a new created anchor object
   * @param anchor
   */
  void PushBack(Anchor *anchor);

  Anchor *Remove(Anchor *anchor);

  void Clear();

  Anchor *first() const { return deque_.first(); }

  Anchor *last() const { return deque_.last(); }

  int count() const { return deque_.count(); }

  Alignment alignment() const { return alignment_; }

 private:

  AbstractView *view;
  Alignment alignment_;

  Deque<Anchor> deque_;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_CONSTRAINT_GROUP_HPP_
