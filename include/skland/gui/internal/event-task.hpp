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

#ifndef SKLAND_GUI_INTERNAL_EVENT_TASK_HPP_
#define SKLAND_GUI_INTERNAL_EVENT_TASK_HPP_

#include "task-base.hpp"

namespace skland {
namespace gui {

class EventTask : public TaskBase {

  EventTask(const EventTask &) = delete;
  EventTask &operator=(const EventTask &) = delete;

 public:

  inline EventTask()
      : TaskBase() {}

  virtual ~EventTask() {}

  virtual void Run(int events = 0) const {
    // override this
  }

};

}
}

#endif // SKLAND_GUI_INTERNAL_EVENT_TASK_NODE_HPP_
