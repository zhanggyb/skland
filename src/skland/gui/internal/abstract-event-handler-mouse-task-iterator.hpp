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

/**
 * @ingroup gui_intern
 * @brief An iterator to get and manage the mouse task in an
 * AbstractEventHandler object
 *
 * This class is used internally.
 */
SKLAND_NO_EXPORT class AbstractEventHandler::MouseTaskIterator {

  MouseTaskIterator() = delete;

  friend bool operator==(const MouseTaskIterator &, const MouseTaskIterator &);

 public:

  /**
   * @brief Constructor
   * @param event_handler A pointer to an AbstractEventHandler object
   *
   * @note The parameter cannot be nullptr.
   */
  MouseTaskIterator(const AbstractEventHandler *event_handler)
      : mouse_task_(&event_handler->p_->mouse_task) {}

  /**
   * @brief Copy constructor
   * @param orig Another iterator
   */
  MouseTaskIterator(const MouseTaskIterator &orig)
      : mouse_task_(orig.mouse_task_) {}

  /**
   * @brief Destructor
   */
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
   * @brief Push the mouse task of the given object at the front of current task
   * @param event_handler
   */
  void PushFront(AbstractEventHandler *event_handler) {
    mouse_task_->PushFront(&event_handler->p_->mouse_task);
  }

  /**
   * @brief Push the mouse task of the given iterator at the front of current task
   * @param other
   */
  void PushFront(const MouseTaskIterator &other) {
    mouse_task_->PushFront(other.mouse_task_);
  }

  /**
   * @brief Push the mouse task of the given object at the back of current task
   * @param event_handler
   */
  void PushBack(AbstractEventHandler *event_handler) {
    mouse_task_->PushBack(&event_handler->p_->mouse_task);
  }

  /**
   * @brief Push the mouse task of the given iterator at the back of current task
   * @param other
   */
  void PushBack(const MouseTaskIterator &other) {
    mouse_task_->PushBack(other.mouse_task_);
  }

  /**
   * @brief A getter method
   * @return A pointer to the mouse task object
   */
  EventTask *mouse_task() const { return mouse_task_; }

  /**
   * @brief A getter method
   * @return The previous task of current one
   */
  EventTask *previous() const {
    return static_cast<EventTask *>(mouse_task_->previous());
  }

  /**
   * @brief A getter method
   * @return The next task of current one
   */
  EventTask *next() const {
    return static_cast<EventTask *>(mouse_task_->next());
  }

  /**
   * @brief Change the current mouse task to the next one it links
   * @return A reference to the current iterator
   *
   * @note Move to the next mouse task may get a nullptr.
   */
  MouseTaskIterator &operator++() {
    mouse_task_ = static_cast<EventTask *>(mouse_task_->next());
    return *this;
  }

  /**
   * @brief Change the current mouse task to the next one it links
   * @return A new iterator object
   *
   * @note Move to the next mouse task may get a nullptr.
   */
  MouseTaskIterator operator++(int) {
    MouseTaskIterator it(*this);
    it.mouse_task_ = static_cast<EventTask *>(mouse_task_->next());
    return it;
  }

  /**
   * @brief Change the current mouse task to the previous one it links
   * @return A reference to the current iterator
   *
   * @note Move to the previous mouse task may get a nullptr.
   */
  MouseTaskIterator &operator--() {
    mouse_task_ = static_cast<EventTask *>(mouse_task_->previous());
    return *this;
  }

  /**
   * @brief Change the current mouse task to the previous one it links
   * @return A new iterator object
   *
   * @note Move to the previous mouse task may get a nullptr.
   */
  MouseTaskIterator operator--(int) {
    MouseTaskIterator it(*this);
    it.mouse_task_ = static_cast<EventTask *>(mouse_task_->previous());
    return it;
  }

  /**
   * @brief Override the bool operator to check if this iterator points to a nullptr
   * @return
   *  - true: this iterator has a valid pointer
   *  - false: this iterator has a nullptr
   */
  operator bool() const { return nullptr != mouse_task_; }

 private:

  EventTask *mouse_task_;

};

bool operator==(const AbstractEventHandler::MouseTaskIterator &it1,
                const AbstractEventHandler::MouseTaskIterator &it2) {
  return it1.mouse_task_ == it2.mouse_task_;
}

}

#endif // SKLAND_GUI_INTERNAL_MOUSE_TASK_PROXY_HPP_
