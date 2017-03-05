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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_MOUSE_TASK_PROXY_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_EVENT_HANDLER_MOUSE_TASK_PROXY_HPP_

#include "abstract-event-handler-private.hpp"

namespace skland {

class AbstractEventHandler::MouseTaskIterator {

  MouseTaskIterator() = delete;

 public:

  MouseTaskIterator(const AbstractEventHandler *event_handler)
      : mouse_task_(&event_handler->p_->mouse_task) {}

  MouseTaskIterator(const MouseTaskIterator &orig)
      : mouse_task_(orig.mouse_task_) {}

  ~MouseTaskIterator() {}

  MouseTaskIterator &operator=(const AbstractEventHandler *event_handler) {
    mouse_task_ = &event_handler->p_->mouse_task;
    return *this;
  }

  MouseTaskIterator &operator=(const MouseTaskIterator &other) {
    mouse_task_ = other.mouse_task_;
    return *this;
  }

  /**
   * @brief Push this mouse task of the given view at the front of this task
   * @param view
   */
  void PushFront(AbstractEventHandler *view) {
    mouse_task_->PushFront(&view->p_->mouse_task);
  }

  void PushFront(const MouseTaskIterator &other) {
    mouse_task_->PushFront(other.mouse_task_);
  }

  /**
   * @brief Push this mouse task of the given view at the back of this task
   * @param event_handler
   */
  void PushBack(AbstractEventHandler *event_handler) {
    mouse_task_->PushBack(&event_handler->p_->mouse_task);
  }

  void PushBack(const MouseTaskIterator &other) {
    mouse_task_->PushBack(other.mouse_task_);
  }

  EventTask *mouse_task() const { return mouse_task_; }

  EventTask *previous() const {
    return static_cast<EventTask *>(mouse_task_->previous());
  }

  EventTask *next() const {
    return static_cast<EventTask *>(mouse_task_->next());
  }

  MouseTaskIterator &operator++() {
    mouse_task_ = static_cast<EventTask *>(mouse_task_->next());
    return *this;
  }

  MouseTaskIterator operator++(int) {
    MouseTaskIterator it(*this);
    it.mouse_task_ = static_cast<EventTask *>(mouse_task_->next());
    return it;
  }

  MouseTaskIterator &operator--() {
    mouse_task_ = static_cast<EventTask *>(mouse_task_->previous());
    return *this;
  }

  MouseTaskIterator operator--(int) {
    MouseTaskIterator it(*this);
    it.mouse_task_ = static_cast<EventTask *>(mouse_task_->previous());
    return it;
  }

  operator bool() const { return nullptr != mouse_task_; }

 private:

  EventTask *mouse_task_;

};

}

#endif // SKLAND_GUI_INTERNAL_MOUSE_TASK_PROXY_HPP_
