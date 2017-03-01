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

#include "event-task.hpp"

#include "abstract-event-handler-private.hpp"

namespace skland {

/**
 * @ingroup gui_intern
 * @brief A proxy class to manipulate mouse task of a view object
 */
class MouseTaskProxy {

 public:

  MouseTaskProxy(AbstractEventHandler *view)
      : event_handler_(view) {}

  MouseTaskProxy(const MouseTaskProxy &orig)
      : event_handler_(orig.event_handler_) {}

  ~MouseTaskProxy() {}

  MouseTaskProxy &operator=(const MouseTaskProxy &other) {
    event_handler_ = other.event_handler_;
    return *this;
  }

  /**
   * @brief Push this mouse task of the given view at the front of this task
   * @param view
   */
  void PushFront(AbstractEventHandler *view) {
    event_handler_->p_->mouse_task.PushFront(&view->p_->mouse_task);
  }

  void PushFront(const MouseTaskProxy &other) {
    event_handler_->p_->mouse_task.PushFront(&other.event_handler_->p_->mouse_task);
  }

  /**
   * @brief Push this mouse task of the given view at the back of this task
   * @param view
   */
  void PushBack(AbstractEventHandler *view) {
    event_handler_->p_->mouse_task.PushBack(&view->p_->mouse_task);
  }

  void PushBack(const MouseTaskProxy &other) {
    event_handler_->p_->mouse_task.PushBack(&other.event_handler_->p_->mouse_task);
  }

  EventTask *GetTask() const {
    return &event_handler_->p_->mouse_task;
  }

  EventTask *GetPreviousTask() const {
    return static_cast<EventTask *>(event_handler_->p_->mouse_task.previous());
  }

  EventTask *GetNextTask() const {
    return static_cast<EventTask *>(event_handler_->p_->mouse_task.next());
  }

  MouseTaskProxy &operator++() {
    event_handler_ =
        static_cast<EventTask *>(event_handler_->p_->mouse_task.next())->event_handler;
    return *this;
  }

  MouseTaskProxy operator++(int) {
    MouseTaskProxy proxy(*this);
    proxy.event_handler_ =
        static_cast<EventTask *>(event_handler_->p_->mouse_task.next())->event_handler;
    return proxy;
  }

  MouseTaskProxy &operator--() {
    event_handler_ =
        static_cast<EventTask *>(event_handler_->p_->mouse_task.previous())->event_handler;
    return *this;
  }

  MouseTaskProxy operator--(int) {
    MouseTaskProxy proxy(*this);
    proxy.event_handler_ =
        static_cast<EventTask *>(event_handler_->p_->mouse_task.previous())->event_handler;
    return proxy;
  }

  AbstractEventHandler *view() const { return event_handler_; }

 private:

  AbstractEventHandler *event_handler_;

};

}

#endif // SKLAND_GUI_INTERNAL_MOUSE_TASK_PROXY_HPP_
