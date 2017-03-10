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

#include "abstract-view-redraw-task.hpp"

#include "abstract-view-private.hpp"

namespace skland {

void AbstractView::RedrawTask::Run() const {
  view->p_->inhibit_redraw = true;

  if (view->p_->geometry_dirty_flag) {
    Rect old = view->p_->pending_geometry;
    view->p_->geometry = view->p_->pending_geometry;
    view->OnGeometryChanged(view->p_->geometry_dirty_flag, old, view->p_->geometry);
    view->p_->geometry_dirty_flag = 0;
  }

  view->OnDraw(&context);
  view->p_->visible = true;

  if (view->p_->is_damaged) {
    context.surface()->Damage(view->p_->damaged_region.x(),
                              view->p_->damaged_region.y(),
                              view->p_->damaged_region.width(),
                              view->p_->damaged_region.height());
    view->p_->is_damaged = false;
  }

  view->p_->inhibit_redraw = false;
}

}
