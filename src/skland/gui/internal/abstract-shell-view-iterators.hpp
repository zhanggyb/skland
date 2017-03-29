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

#ifndef SKLAND_ABSTRACT_SHELL_VIEW_ITERATORS_HPP
#define SKLAND_ABSTRACT_SHELL_VIEW_ITERATORS_HPP

#include "abstract-shell-view-private.hpp"

namespace skland {

/**
 * @ingroup gui_intern
 * @brief An iterator class to access shell view stack
 */
SKLAND_NO_EXPORT class AbstractShellView::Iterator {

 public:

  Iterator(AbstractShellView *shell_view = nullptr)
      : shell_view_(shell_view) {}

  Iterator(const Iterator &orig)
      : shell_view_(orig.shell_view_) {}

  ~Iterator() {}

  Iterator &operator=(const Iterator &other) {
    shell_view_ = other.shell_view_;
    return *this;
  }

  Iterator &operator=(AbstractShellView *shell_view) {
    shell_view_ = shell_view;
    return *this;
  }

  AbstractShellView *parent() const { return shell_view_->p_->parent; }

 private:

  AbstractShellView *shell_view_;

};

/**
 * @ingroup gui_intern
 * @brief A const iterator class to access shell view stack<
 */
SKLAND_NO_EXPORT class AbstractShellView::ConstIterator {

 public:

  ConstIterator(const AbstractShellView *shell_view = nullptr)
      : shell_view_(shell_view) {}

  ConstIterator(const ConstIterator &orig)
      : shell_view_(orig.shell_view_) {}

  ~ConstIterator() {}

  ConstIterator &operator=(const ConstIterator &other) {
    shell_view_ = other.shell_view_;
    return *this;
  }

  ConstIterator &operator=(const AbstractShellView *shell_view) {
    shell_view_ = shell_view;
    return *this;
  }

  const AbstractShellView *parent() const { return shell_view_->p_->parent; }

 private:

  const AbstractShellView *shell_view_;

};

}

#endif //SKLAND_ABSTRACT_SHELL_VIEW_ITERATORS_HPP
