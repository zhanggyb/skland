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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_ITERATOR_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_ITERATOR_HPP_

#include <skland/gui/abstract-view.hpp>
#include "abstract-view-private.hpp"

namespace skland {

class AbstractView::Iterator {

 public:

  Iterator(AbstractView *view = nullptr)
      : view_(view) {}

  Iterator(const Iterator &orig)
      : view_(orig.view_) {}

  ~Iterator() {}

  Iterator &operator=(const Iterator &other) {
    view_ = other.view_;
    return *this;
  }

  Iterator &operator=(AbstractView *view) {
    view_ = view;
    return *this;
  }

  Iterator &operator++() {
    view_ = view_->p_->next;
    return *this;
  }

  Iterator operator++(int) {
    Iterator it(view_->p_->next);
    return it;
  }

  Iterator &operator--() {
    view_ = view_->p_->previous;
    return *this;
  }

  Iterator operator--(int) {
    Iterator it(view_->p_->previous);
    return it;
  }

  Surface *GetSurface() const { return view_->GetSurface(view_); }

  AbstractView *parent() const { return view_->p_->parent; }

  AbstractView *previous() const { return view_->p_->previous; }

  AbstractView *next() const { return view_->p_->next; }

  AbstractView *first_child() const { return view_->p_->first_child; }

  AbstractView *last_child() const { return view_->p_->last_child; }

  AbstractLayout *layout() const { return view_->p_->layout; }

  AbstractView *view() const { return view_; }

  operator bool() const { return nullptr != view_; }

 private:

  AbstractView *view_;

};

class AbstractView::ConstIterator {

 public:

  ConstIterator(const AbstractView *view = nullptr)
      : view_(view) {}

  ConstIterator(const ConstIterator &orig)
      : view_(orig.view_) {}

  ~ConstIterator() {}

  ConstIterator &operator=(const ConstIterator &other) {
    view_ = other.view_;
    return *this;
  }

  ConstIterator &operator=(AbstractView *view) {
    view_ = view;
    return *this;
  }

  ConstIterator &operator++() {
    view_ = view_->p_->next;
    return *this;
  }

  ConstIterator operator++(int) {
    ConstIterator it(view_->p_->next);
    return it;
  }

  ConstIterator &operator--() {
    view_ = view_->p_->previous;
    return *this;
  }

  ConstIterator operator--(int) {
    ConstIterator it(view_->p_->previous);
    return it;
  }

  const AbstractView *parent() const { return view_->p_->parent; }

  const AbstractView *previous() const { return view_->p_->previous; }

  const AbstractView *next() const { return view_->p_->next; }

  const AbstractView *first_child() const { return view_->p_->first_child; }

  const AbstractView *last_child() const { return view_->p_->last_child; }

  const AbstractView *view() const { return view_; }

  operator bool() const { return nullptr != view_; }

 private:

  const AbstractView *view_;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_VIEW_ITERATOR_HPP_
