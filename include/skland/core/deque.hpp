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

#include "export.hpp"

namespace skland {
namespace core {

class Deque {

 public:

  class Element {

    friend class Deque;

   public:

    Element();

    virtual ~Element();

   protected:

    Element *previous() const { return previous_; }

    Element *next() const { return next_; }

    Deque *deque() const { return deque_; }

   private:

    Element *previous_;
    Element *next_;
    Deque *deque_;

  };

  class Iterator {

   public:

    explicit Iterator(Element *element = nullptr)
        : p_(element) {}

    Iterator(const Iterator &orig)
        : p_(orig.p_) {}

    ~Iterator() {}

    Iterator &operator=(const Iterator &other) {
      p_ = other.p_;
      return *this;
    }

    Iterator &operator++() {
      p_ = p_->next_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator retval;
      retval.p_ = p_->next_;
      return retval;
    }

    Iterator &operator--() {
      p_ = p_->previous_;
      return *this;
    }

    Iterator operator--(int) {
      Iterator retval;
      retval.p_ = p_->previous_;
      return retval;
    }

    bool operator==(const Iterator &other) const {
      return p_ == other.p_;
    }

    bool operator==(const Element *element) const {
      return p_ == element;
    }

    bool operator!=(const Iterator &other) const {
      return p_ != other.p_;
    }

    bool operator!=(const Element *element) const {
      return p_ != element;
    }

    template<typename T>
    T *cast() const {
      return static_cast<T *>(p_);
    }

   private:

    Element *p_;

  };

  Deque();

  ~Deque();

  void PushFront(Element *item);

  void PushBack(Element *item);

  void Insert(Element *item, int index = 0);

  Element *Remove(Element *item);

  void Clear();

  Element *operator[](int index) const;

  Iterator begin() const {
    return Iterator(first_);
  }

  Iterator end() const {
    return Iterator(nullptr);
  }

  int count() const { return count_; }

 protected:

  Element *first() const { return first_; }

  Element *last() const { return last_; }

 private:

  Element *first_;
  Element *last_;
  int count_;

};

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_DEQUE_HPP_
