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

#ifndef SKLAND_GUI_ANCHOR_GROUP_HPP_
#define SKLAND_GUI_ANCHOR_GROUP_HPP_

#include "../core/types.hpp"

namespace skland {

class AbstractView;
class Anchor;

/**
 * @ingroup gui_intern
 * @brief Anchor group
 */
class AnchorGroup {

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

  AbstractView *view() const { return view_; }

  Alignment alignment() const { return alignment_; }

  Anchor *first() const { return first_; }

  Anchor *last() const { return last_; }

  int count() const { return count_; }

 private:

  AbstractView *view_;
  Alignment alignment_;

  Anchor *first_;
  Anchor *last_;
  int count_;

};

}

#endif // SKLAND_GUI_ANCHOR_GROUP_HPP_
