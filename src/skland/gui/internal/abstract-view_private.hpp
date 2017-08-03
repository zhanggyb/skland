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

#include "skland/gui/abstract-view.hpp"

#include "skland/core/padding.hpp"
#include "skland/gui/anchor.hpp"
#include "skland/gui/anchor-group.hpp"

namespace skland {
namespace gui {

/**
 * @ingroup gui_intern
 * @brief A structure for private data in AbstractView
 */
SKLAND_NO_EXPORT struct AbstractView::Private {

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  explicit Private(AbstractView *view)
      : previous(nullptr),
        next(nullptr),
        first_child(nullptr),
        last_child(nullptr),
        parent(nullptr),
        children_count(0),
        shell_view(nullptr),
        visible(true),
        minimal_size(0, 0),
        preferred_size(100, 100),
        maximal_size(65536, 65536),
        x_layout_policy(kLayoutPreferred),
        y_layout_policy(kLayoutPreferred),
        geometry_dirty_flags(0),
        geometry_task(view),
        redraw_task(view),
        left_anchor_group(view, kAlignLeft),
        top_anchor_group(view, kAlignTop),
        right_anchor_group(view, kAlignRight),
        bottom_anchor_group(view, kAlignBottom),
        need_layout(false),
        is_layouting(false),
        layout(nullptr) {}

  ~Private() = default;

  AbstractView *previous;
  AbstractView *next;

  AbstractView *first_child;
  AbstractView *last_child;

  AbstractView *parent;
  int children_count;

  AbstractShellView *shell_view;

  bool visible;

  Size minimal_size;

  Size preferred_size;

  Size maximal_size;

  LayoutPolicy x_layout_policy;

  LayoutPolicy y_layout_policy;

  Padding padding;

  /**
   * @brief Bitwise data to indicate if the position or size need to update
   *
   * Use GeometryTypeMask to check this value
   */
  int geometry_dirty_flags;

  RectF geometry;

  RectF last_geometry;

  GeometryTask geometry_task;

  RedrawTask redraw_task;

  AnchorGroup left_anchor_group;
  AnchorGroup top_anchor_group;
  AnchorGroup right_anchor_group;
  AnchorGroup bottom_anchor_group;

  bool need_layout;
  bool is_layouting;
  AbstractLayout *layout;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_PRIVATE_HPP_
