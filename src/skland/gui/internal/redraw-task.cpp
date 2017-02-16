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

#include "redraw-task.hpp"
#include <skland/gui/abstract-view.hpp>

namespace skland {

void RedrawTask::Run() const {
  view->OnDraw(&context);
  view->visible_ = true;

  if (view->is_damaged_) {
    context.view_surface()->Damage(view->damaged_region_.x(),
                              view->damaged_region_.y(),
                              view->damaged_region_.width(),
                              view->damaged_region_.height());
    view->is_damaged_ = false;
  }
}

}
