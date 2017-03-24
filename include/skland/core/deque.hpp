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

#ifndef SKLAND_CORE_DEQUE_HPP_
#define SKLAND_CORE_DEQUE_HPP_

#include "defines.hpp"

namespace skland {

/**
 * @ingroup core
 * @brief A template class to implement a deque container
 * @tparam T A typename indicate an class stored in this container
 *
 * Example code to declare an item class can be used in this deque:
 *
 * @code
 * class Item {
 *
 *  template<typename T> friend class Deque;
 *
 *  public:
 *
 *    Item(): previous_(nullptr), next_(nullptr), deque_(nullptr) {}
 *
 *    ~Item() {
 *      if (deque_) deque_->Remove(this);
 *    }
 *
 *  private:
 *
 *    Item *previous_;
 *    Item *next_;
 *    Deque<Item>* deque_;
 *
 * }
 * @endcode
 */
template<typename T>
class Deque {

 public:

  Deque();

  ~Deque();

  void PushFront(T *item);

  void PushBack(T *item);

  void Insert(T *item, int index = 0);

  T *Remove(T *item);

  void Clear();

  T *operator[](int index) const;

  T *first() const { return first_; }

  T *last() const { return last_; }

  int count() const { return count_; }

 private:

  T *first_;
  T *last_;
  int count_;

};

template<typename T>
Deque<T>::Deque()
    : first_(nullptr), last_(nullptr), count_(0) {}

template<typename T>
Deque<T>::~Deque() {
  Clear();
}

template<typename T>
void Deque<T>::PushFront(T *item) {
  DBG_ASSERT(nullptr == item->deque_);
  DBG_ASSERT(nullptr == item->previous_);
  DBG_ASSERT(nullptr == item->next_);

  if (first_) {
    first_->previous_ = item;
    item->next_ = first_;
  } else {
    DBG_ASSERT(nullptr == last_);
    DBG_ASSERT(0 == count_);
  }
  first_ = item;

  item->deque_ = this;
  count_++;
}

template<typename T>
void Deque<T>::PushBack(T *item) {
  DBG_ASSERT(nullptr == item->deque_);
  DBG_ASSERT(nullptr == item->previous_);
  DBG_ASSERT(nullptr == item->next_);

  if (last_) {
    last_->next_ = item;
    item->previous_ = last_;
  } else {
    DBG_ASSERT(nullptr == first_);
    DBG_ASSERT(0 == count_);
    first_ = item;
  }
  last_ = item;

  item->deque_ = this;
  count_++;
}

template<typename T>
void Deque<T>::Insert(T *item, int index) {
  DBG_ASSERT(nullptr == item->deque_);
  DBG_ASSERT(nullptr == item->previous_);
  DBG_ASSERT(nullptr == item->next_);

  if (nullptr == first_) {
    DBG_ASSERT(nullptr == last_);
    DBG_ASSERT(0 == count_);
    last_ = item;
    first_ = item;
  } else {
    if (index >= 0) {
      T *p = first_;
      while (p && (index > 0)) {
        p = p->next_;
        index--;
      }
      if (p) {  // insert before p
        item->previous_ = p->previous_;
        item->next_ = p;
        if (p->previous_) p->previous_->next_ = item;
        else first_ = item;
        p->previous_ = item;
      } else {  // push back
        last_->next_ = item;
        item->previous_ = last_;
        last_ = item;
      }
    } else {
      T *p = last_;
      while (p && (index < -1)) {
        p = p->previous_;
        index++;
      }
      if (p) {  // insert after p
        item->next_ = p->next_;
        item->previous_ = p;
        if (p->next_) p->next_->previous_ = item;
        else last_ = item;
        p->next_ = item;
      } else {  // push front
        first_->previous_ = item;
        item->next_ = first_;
        first_ = item;
      }
    }
  }

  item->deque_ = this;
  count_++;
}

template<typename T>
T *Deque<T>::Remove(T *item) {
  if (item->deque_ != this) return nullptr;

  DBG_ASSERT(count_ > 0);

  if (item->previous_) {
    item->previous_->next_ = item->next_;
  } else {
    DBG_ASSERT(first_ == item);
    first_ = item->next_;
  }

  if (item->next_) {
    item->next_->previous_ = item->previous_;
  } else {
    DBG_ASSERT(last_ == item);
    last_ = item->previous_;
  }

  count_--;
  DBG_ASSERT(count_ >= 0);

  item->previous_ = nullptr;
  item->next_ = nullptr;
  item->deque_ = nullptr;

  return item;
}

template<typename T>
void Deque<T>::Clear() {
  T *ptr = first_;
  T *next = nullptr;

  while (ptr) {
    next = ptr->next_;
    ptr->previous_ = nullptr;
    ptr->next_ = nullptr;
    ptr->deque_ = nullptr;
    delete ptr;
    ptr = next;
  }

  count_ = 0;
  first_ = nullptr;
  last_ = nullptr;
}

template<typename T>
T *Deque<T>::operator[](int index) const {
  if (index < 0) index = count_ + index;

  T *item = nullptr;
  int mid = (count_ + 1) / 2;

  if (index >= 0 && index < mid) {
    item = first_;
    while (index > 0) {
      item = item->next_;
      index--;
    }
  } else if (index >= mid && index < count_) {
    item = last_;
    int max = count_ - 1;
    while (index < max) {
      item = item->previous_;
      index++;
    }
  }

  return item;
}

}

#endif // SKLAND_CORE_DEQUE_HPP_
