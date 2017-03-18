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

#include "abstract-view-redraw-task.hpp"

#include "abstract-view-constraint.hpp"
#include "abstract-view-constraint-group.hpp"

namespace skland {

class AbstractShellView;
class AbstractLayout;

SKLAND_NO_EXPORT struct AbstractView::Private {

  Private() = delete;
  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  enum DirtyFlagMask {
    kDirtyLeftMask = 0x1 << 0,
    kDirtyTopMask = 0x1 << 1,
    kDirtyRightMask = 0x1 << 2,
    kDirtyBottomMask = 0x1 << 3
  };

  Private(AbstractView *view)
      : previous(nullptr),
        next(nullptr),
        first_child(nullptr),
        last_child(nullptr),
        parent(nullptr),
        children_count(0),
        shell(nullptr),
        visible(true),
        dirty_flag(0),
        redraw_task(view),
        is_damaged(false),
        inhibit_redraw(false),
        layout(nullptr),
        left_constraint_group(kAlignLeft),
        top_constraint_group(kAlignTop),
        right_constraint_group(kAlignRight),
        bottom_constraint_group(kAlignBottom) {}

  ~Private() {}

  AbstractView *previous;
  AbstractView *next;

  AbstractView *first_child;
  AbstractView *last_child;

  AbstractView *parent;
  int children_count;

  AbstractShellView *shell;

  bool visible;

  /**
   * @brief Bitwise data to indicate if the position or size need to update
   *
   * Use GeometryTypeMask to check this value
   */
  int dirty_flag;

  Rect geometry;

  Rect last_geometry;

  RedrawTask redraw_task;

  /**
   * @brief If need to call wayland API to damage area on the surface
   */
  bool is_damaged;

  /**
   * @brief The damage region
   *
   * This member variable works with is_damaged.
   */
  RectI damaged_region;

  bool inhibit_redraw;

  AbstractLayout *layout;

  ConstraintGroup left_constraint_group;
  ConstraintGroup top_constraint_group;
  ConstraintGroup right_constraint_group;
  ConstraintGroup bottom_constraint_group;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
