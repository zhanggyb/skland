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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_PRIVATE_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_PRIVATE_HPP_

#include <skland/gui/abstract-event-handler.hpp>
#include <skland/core/rect.hpp>

#include "abstract-event-handler-event-task.hpp"
#include "abstract-event-handler-redraw-task.hpp"

namespace skland {

struct AbstractEventHandler::Private {

  Private(AbstractEventHandler *event_handler)
      : mouse_task(event_handler),
        mouse_motion_task(event_handler),
        redraw_task(event_handler),
        is_damaged_(false) {}

  ~Private() {}

  EventTask mouse_task;
  EventTask mouse_motion_task;

  RedrawTask redraw_task;

  // Damage area
  bool is_damaged_;
  RectI damaged_region_;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_PRIVATE_HPP_
