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

#ifndef SKLAND_GUI_INTERNAL_MOUSE_TASK_PROXY_HPP_
#define SKLAND_GUI_INTERNAL_MOUSE_TASK_PROXY_HPP_

namespace skland {

class AbstractView;

class MouseTaskProxy {

 public:

  MouseTaskProxy(AbstractView *view)
      : view_(view) {}

  ~MouseTaskProxy() {}

  /**
   * @brief Push this mouse task of the given view at the front of this task
   * @param view
   */
  void PushFront(AbstractView *view);

  void PushFront(const MouseTaskProxy &other);

  /**
   * @brief Push this mouse task of the given view at the back of this task
   * @param view
   */
  void PushBack(AbstractView *view);

  void PushBack(const MouseTaskProxy &other);

  bool HasPreviousTask() const;

  bool HasNextTask() const;

 private:

  AbstractView *view_;

};

}

#endif // SKLAND_GUI_INTERNAL_MOUSE_TASK_PROXY_HPP_
