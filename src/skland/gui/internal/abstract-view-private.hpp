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

namespace skland {

class AbstractShellView;

SKLAND_NO_EXPORT struct AbstractView::Private {

  Private() = delete;
  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  enum GeometryDirtyFlagMask {
    kPositionMask = 0x1 << 0,
    kSizeMask = 0x1 << 1
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
        geometry_dirty_flag(0),
        redraw_task(view),
        is_damaged(false),
        inhibit_redraw(false) {}

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
  int geometry_dirty_flag;

  Rect geometry;

  Rect saved_geometry;

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
};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
