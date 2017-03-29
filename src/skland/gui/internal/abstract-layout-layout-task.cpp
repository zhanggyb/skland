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

#include "abstract-layout-layout-task.hpp"

#include "abstract-layout-private.hpp"

namespace skland {

void AbstractLayout::LayoutTask::Run() const {
  layout->p_->is_layouting = true;
  layout->OnLayout(0,
                   layout->p_->padding.left,
                   layout->p_->padding.top,
                   layout->p_->padding.right,
                   layout->p_->padding.bottom);
  layout->p_->is_layouting = false;
}

}