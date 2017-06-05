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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_REDRAW_TASK_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_REDRAW_TASK_HPP_

#include <skland/gui/abstract-shell-view.hpp>

#include <skland/gui/task.hpp>
#include <skland/gui/context.hpp>

namespace skland {

/**
 * @ingroup gui_intern
 * @brief A task structure to process shell view rendering in event loop
 */
SKLAND_NO_EXPORT struct AbstractShellView::RedrawTask : public Task {

  RedrawTask() = delete;
  RedrawTask(const RedrawTask &) = delete;
  RedrawTask &operator=(const RedrawTask &) = delete;

  RedrawTask(AbstractShellView *event_handler)
      : Task(), shell_view(event_handler) {}

  virtual ~RedrawTask() {}

  virtual void Run() const final;

  AbstractShellView *shell_view;
  Context context;

  static RedrawTask *Get(const AbstractShellView *shell_view);

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_REDRAW_TASK_HPP_
