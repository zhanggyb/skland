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

#include "abstract-view-redraw-task.hpp"

#include "skland/gui/anchor.hpp"
#include "skland/gui/anchor-group.hpp"

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
    kDirtyBottomMask = 0x1 << 3,
    kDirtyWidthMask = 0x1 << 4,
    kDirtyHeightMask = 0x1 << 5
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
        minimal_size(0, 0),
        preferred_size(100, 100),
        maximal_size(65536, 65536),
        x_layout_policy(kLayoutPreferred),
        y_layout_policy(kLayoutPreferred),
        dirty_flag(0),
        redraw_task(view),
        is_damaged(false),
        inhibit_redraw(false),
        left_anchor_group(view, kAlignLeft),
        top_anchor_group(view, kAlignTop),
        right_anchor_group(view, kAlignRight),
        bottom_anchor_group(view, kAlignBottom),
        layout(nullptr) {}

  ~Private() {}

  AbstractView *previous;
  AbstractView *next;

  AbstractView *first_child;
  AbstractView *last_child;

  AbstractView *parent;
  int children_count;

  AbstractShellView *shell;

  bool visible;

  Size minimal_size;

  Size preferred_size;

  Size maximal_size;

  LayoutPolicy x_layout_policy;

  LayoutPolicy y_layout_policy;

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

  AnchorGroup left_anchor_group;
  AnchorGroup top_anchor_group;
  AnchorGroup right_anchor_group;
  AnchorGroup bottom_anchor_group;

  AbstractLayout *layout;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
