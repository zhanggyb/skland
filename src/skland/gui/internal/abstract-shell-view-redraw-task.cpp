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

#include "abstract-shell-view-redraw-task.hpp"

#include "abstract-shell-view-private.hpp"
#include "abstract-event-handler-private.hpp"

namespace skland {

void AbstractShellView::RedrawTask::Run() const {
  shell_view->OnDraw(&context);

  AbstractEventHandler *base = static_cast<AbstractEventHandler *>(shell_view);

  if (base->p_->is_damaged) {
    context.surface()->Damage(base->p_->damaged_region.x(),
                              base->p_->damaged_region.y(),
                              base->p_->damaged_region.width(),
                              base->p_->damaged_region.height());
    base->p_->is_damaged = false;
  }
}

}
