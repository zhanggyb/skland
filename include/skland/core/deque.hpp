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
namespace core {

/**
 * @ingroup core
 * @brief A faster deque container
 */
class Deque {

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Deque);

  /**
   * @brief A nested class represents an element in a deque
   */
  class Element {

    friend class Deque;

   public:

    SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Element);

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

  /**
   * @brief A nested iterator for deque
   */
  class Iterator {

   public:

    explicit Iterator(Element *element = nullptr)
        : ptr_(element) {}

    Iterator(const Iterator &orig) = default;

    ~Iterator() = default;

    Iterator &operator=(const Iterator &other) = default;

    Iterator &operator++() {
      ptr_ = ptr_->next_;
      return *this;
    }

    Iterator operator++(int) {
      Iterator retval;
      retval.ptr_ = ptr_->next_;
      return retval;
    }

    Iterator &operator--() {
      ptr_ = ptr_->previous_;
      return *this;
    }

    Iterator operator--(int) {
      Iterator retval;
      retval.ptr_ = ptr_->previous_;
      return retval;
    }

    bool operator==(const Iterator &other) const {
      return ptr_ == other.ptr_;
    }

    bool operator==(const Element *element) const {
      return ptr_ == element;
    }

    bool operator!=(const Iterator &other) const {
      return ptr_ != other.ptr_;
    }

    bool operator!=(const Element *element) const {
      return ptr_ != element;
    }

    template<typename T>
    T *cast() const {
      return static_cast<T *>(ptr_);
    }

    explicit operator bool() const {
      return nullptr != ptr_;
    }

   private:

    Element *ptr_;

  };

  Deque();

  virtual ~Deque();

  void PushFront(Element *item);

  void PushBack(Element *item);

  void Insert(Element *item, int index = 0);

  Element *Remove(Element *item);

  void Clear();

  Element *operator[](int index) const;

  Iterator begin() const {
    return Iterator(first_);
  }

  Iterator rbegin() const {
    return Iterator(last_);
  }

  Iterator end() const {
    return Iterator(last_->next_);
  }

  Iterator rend() const {
    return Iterator(first_->previous_);
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
