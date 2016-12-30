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

#ifndef SKLAND_GUI_INTERNAL_WIDGET_DRAW_TASK_HPP_
#define SKLAND_GUI_INTERNAL_WIDGET_DRAW_TASK_HPP_

#include "view-task.hpp"

namespace skland {

class AbstractWidget;
class Canvas;

struct WidgetDrawTask : public ViewTask {
  WidgetDrawTask(const WidgetDrawTask &) = delete;
  WidgetDrawTask &operator=(const WidgetDrawTask &) = delete;

  WidgetDrawTask(AbstractWidget *widget = nullptr, Canvas *canvas = nullptr);

  virtual ~WidgetDrawTask() {}

  virtual void Run() const final;

  Canvas *canvas;
};

}

#endif // SKLAND_GUI_INTERNAL_WIDGET_DRAW_TASK_HPP_
