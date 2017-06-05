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

#include "abstract-view_redraw-task.hpp"

#include "abstract-view_private.hpp"

namespace skland {

void AbstractView::RedrawTask::Run() const {
  view->p_->is_drawing = true;

  if (view->p_->dirty_flag) {
    view->OnGeometryChange(view->p_->dirty_flag, view->p_->last_geometry, view->p_->geometry);
    view->p_->last_geometry = view->p_->geometry;
  }

  if (view->p_->need_layout) {
    view->p_->is_layouting = true;
    view->OnLayout(view->p_->dirty_flag,
                   view->p_->padding.left,
                   view->p_->padding.top,
                   static_cast<int>(view->p_->geometry.width()) - view->p_->padding.right,
                   static_cast<int>(view->p_->geometry.height()) - view->p_->padding.bottom);
    view->p_->is_layouting = false;
  }

  if (view->p_->need_redraw)   // This boolean flag is reset in AbstractLayout
    view->OnDraw(&context);

  if (view->p_->is_damaged) {
    context.surface()->Damage(view->p_->damaged_region.x(),
                              view->p_->damaged_region.y(),
                              view->p_->damaged_region.width(),
                              view->p_->damaged_region.height());
  }

  view->p_->dirty_flag = 0;
  view->p_->need_layout = false;
  view->p_->is_damaged = false;
  view->p_->visible = true;

  view->p_->is_drawing = false;
}

AbstractView::RedrawTask *AbstractView::RedrawTask::Get(const AbstractView *view) {
  return &view->p_->redraw_task;
}

}
