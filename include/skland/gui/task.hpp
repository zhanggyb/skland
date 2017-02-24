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

#ifndef SKLAND_GUI_TASK_HPP_
#define SKLAND_GUI_TASK_HPP_

#include "../core/defines.hpp"

namespace skland {

SKLAND_EXPORT class Task {

  Task(const Task &) = delete;
  Task &operator=(const Task &) = delete;

 public:

  Task()
      : previous_(nullptr), next_(nullptr) {}

  virtual ~Task();

  bool IsLinked() const {
    return (nullptr != previous_) || (nullptr != next_);
  }

  void PushBack(Task *other);

  void PushFront(Task *other);

  void Unlink();

  virtual void Run() const {
    // override this
  }

  Task *previous() const {
    return previous_;
  }

  Task *next() const {
    return next_;
  }

 private:

  Task *previous_;
  Task *next_;

};

} // namespace skland

#endif // SKLAND_GUI_TASK_HPP_
