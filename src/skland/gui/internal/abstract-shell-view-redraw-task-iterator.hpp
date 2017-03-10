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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_REDRAW_TASK_PROXY_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_REDRAW_TASK_PROXY_HPP_

#include "abstract-shell-view-private.hpp"

namespace skland {

class AbstractShellView::RedrawTaskIterator {

  RedrawTaskIterator() = delete;

 public:

  RedrawTaskIterator(AbstractShellView *shell_view)
      : redraw_task_(&shell_view->p_->redraw_task) {}

  RedrawTaskIterator(const RedrawTaskIterator &orig)
      : redraw_task_(orig.redraw_task_) {}

  ~RedrawTaskIterator() {}

  RedrawTaskIterator &operator=(const AbstractShellView *shell_view) {
    redraw_task_ = &shell_view->p_->redraw_task;
    return *this;
  }

  RedrawTaskIterator &operator=(const RedrawTaskIterator &other) {
    redraw_task_ = other.redraw_task_;
    return *this;
  }

  void SetContext(const Context &context) {
    redraw_task_->context = context;
  }

  const Context &context() const {
    return redraw_task_->context;
  }

  bool IsLinked() const {
    return redraw_task_->IsLinked();
  }

  void PushBack(AbstractShellView *shell_view) {
    redraw_task_->PushBack(&shell_view->p_->redraw_task);
  }

  void PushFront(AbstractShellView *shell_view) {
    redraw_task_->PushFront(&shell_view->p_->redraw_task);
  }

  RedrawTask *redraw_task() const { return redraw_task_; }

  RedrawTask *previous() const {
    return static_cast<RedrawTask *>(redraw_task_->previous());
  }

  RedrawTask *next() const {
    return static_cast<RedrawTask * >(redraw_task_->next());
  }

  RedrawTaskIterator &operator++() {
    redraw_task_ = static_cast<RedrawTask *>(redraw_task_->next());
    return *this;
  }

  RedrawTaskIterator operator++(int) {
    RedrawTaskIterator it(*this);
    it.redraw_task_ = static_cast<RedrawTask *>(redraw_task_->next());
    return it;
  }

  RedrawTaskIterator &operator--() {
    redraw_task_ = static_cast<RedrawTask *>(redraw_task_->previous());
    return *this;
  }

  RedrawTaskIterator operator--(int) {
    RedrawTaskIterator it(*this);
    it.redraw_task_ = static_cast<RedrawTask *>(redraw_task_->previous());
    return it;
  }

  void PushToHead() {
    kRedrawTaskHead.PushBack(redraw_task_);
  }

  void PushToTail() {
    kRedrawTaskTail.PushFront(redraw_task_);
  }

  operator bool() const { return nullptr != redraw_task_; }

 private:

  RedrawTask *redraw_task_;

};

} // namespace skland

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_REDRAW_TASK_PROXY_HPP_
