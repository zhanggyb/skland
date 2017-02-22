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

#include "view-task.hpp"
#include <skland/gui/abstract-view.hpp>

namespace skland {

/**
 * @ingroup gui_intern
 * @brief A proxy class to manipulate mouse task of a view object
 */
class MouseTaskProxy {

 public:

  MouseTaskProxy(AbstractView *view)
      : view_(view) {}

  MouseTaskProxy(const MouseTaskProxy &orig)
      : view_(orig.view_) {}

  ~MouseTaskProxy() {}

  MouseTaskProxy &operator=(const MouseTaskProxy &other) {
    view_ = other.view_;
    return *this;
  }

  /**
   * @brief Push this mouse task of the given view at the front of this task
   * @param view
   */
  void PushFront(AbstractView *view) {
    view_->mouse_task_->PushFront(view->mouse_task_.get());
  }

  void PushFront(const MouseTaskProxy &other) {
    view_->mouse_task_->PushFront(other.view_->mouse_task_.get());
  }

  /**
   * @brief Push this mouse task of the given view at the back of this task
   * @param view
   */
  void PushBack(AbstractView *view) {
    view_->mouse_task_->PushBack(view->mouse_task_.get());
  }

  void PushBack(const MouseTaskProxy &other) {
    view_->mouse_task_->PushBack(other.view_->mouse_task_.get());
  }

  ViewTask *GetTask() const {
    return view_->mouse_task_.get();
  }

  ViewTask *GetPreviousTask() const {
    return static_cast<ViewTask *>(view_->mouse_task_->previous());
  }

  ViewTask *GetNextTask() const {
    return static_cast<ViewTask *>(view_->mouse_task_->next());
  }

  MouseTaskProxy &operator++() {
    view_ = static_cast<ViewTask *>(view_->mouse_task_->next())->view;
    return *this;
  }

  MouseTaskProxy operator++(int) {
    MouseTaskProxy proxy(*this);
    proxy.view_ = static_cast<ViewTask *>(view_->mouse_task_->next())->view;
    return proxy;
  }

  MouseTaskProxy &operator--() {
    view_ = static_cast<ViewTask *>(view_->mouse_task_->previous())->view;
    return *this;
  }

  MouseTaskProxy operator--(int) {
    MouseTaskProxy proxy(*this);
    proxy.view_ = static_cast<ViewTask *>(view_->mouse_task_->previous())->view;
    return proxy;
  }

  AbstractView *view() const { return view_; }

 private:

  AbstractView *view_;

};

}

#endif // SKLAND_GUI_INTERNAL_MOUSE_TASK_PROXY_HPP_
