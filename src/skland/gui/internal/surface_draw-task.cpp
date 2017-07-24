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

#include "surface_draw-task.hpp"

#include "surface_private.hpp"

namespace skland {
namespace gui {

void Surface::DrawTask::Run() const {
  Task *task = nullptr;
  while (surface->p_->draw_task_head.next() != &surface->p_->draw_task_tail) {
    task = surface->p_->draw_task_head.next();
    task->Unlink();

    // TODO: not run in this way:
    task->Run();
  }
}

} // namespace gui
} // namespace skland
