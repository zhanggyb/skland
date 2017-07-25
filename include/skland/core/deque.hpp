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

#include <cstddef>

namespace skland {
namespace core {

/**
 * @ingroup core
 * @brief A simple double-ended queue container
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

    Element()
        : previous_(nullptr),
          next_(nullptr) {}

    virtual ~Element();

    bool IsLinked() const {
      return (nullptr != previous_) || (nullptr != next_);
    }

    /**
     * @brief Push another element to the front
     * @param other
     */
    void PushFront(Element *other);

    /**
     * @brief Push another element to the back
     * @param other
     */
    void PushBack(Element *other);

    void Unlink();

   protected:

    /**
     * @brief Getter of previous_
     * @return
     */
    Element *previous() const { return previous_; }

    /**
     * @brief Getter of next_
     * @return
     */
    Element *next() const { return next_; }

   private:

    Element *previous_;
    Element *next_;

  };

  /**
   * @brief A nested iterator for deque
   */
  class Iterator {

   public:

    explicit Iterator(Element *element = nullptr)
        : element_(element) {}

    Iterator(const Iterator &orig) = default;

    ~Iterator() = default;

    Iterator &operator=(const Iterator &other) = default;

    Iterator &operator++() {
      element_ = element_->next_;
      return *this;
    }

    Iterator operator++(int) {
      return Iterator(element_->next_);
    }

    Iterator &operator--() {
      element_ = element_->previous_;
      return *this;
    }

    Iterator operator--(int) {
      return Iterator(element_->previous_);
    }

    void PushFront(Element *element) {
      element_->PushFront(element);
    }

    void PushBack(Element *element) {
      element_->PushBack(element);
    }

    void Remove() {
      element_->Unlink();
    }

    bool operator==(const Iterator &other) const {
      return element_ == other.element_;
    }

    bool operator==(const Element *element) const {
      return element_ == element;
    }

    bool operator!=(const Iterator &other) const {
      return element_ != other.element_;
    }

    bool operator!=(const Element *element) const {
      return element_ != element;
    }

    Element *element() const {
      return element_;
    }

    template<typename T>
    T *cast() const {
      return static_cast<T *>(element_);
    }

    explicit operator bool() const {
      return nullptr != element_;
    }

   private:

    Element *element_;

  };

  class ConstIterator {

   public:

    explicit ConstIterator(const Element *element)
        : element_(element) {}

    ConstIterator(const ConstIterator &orig) = default;

    ~ConstIterator() = default;

    ConstIterator &operator=(const ConstIterator &other) = default;

    ConstIterator &operator++() {
      element_ = element_->next_;
      return *this;
    }

    ConstIterator operator++(int) {
      return ConstIterator(element_->next_);
    }

    ConstIterator &operator--() {
      element_ = element_->previous_;
      return *this;
    }

    ConstIterator operator--(int) {
      return ConstIterator(element_->previous_);
    }

    bool operator==(const ConstIterator &other) const {
      return element_ == other.element_;
    }

    bool operator==(const Element *element) const {
      return element_ == element;
    }

    bool operator!=(const ConstIterator &other) const {
      return element_ != other.element_;
    }

    bool operator!=(const Element *element) const {
      return element_ != element;
    }

    const Element *element() const {
      return element_;
    }

    explicit operator bool() const {
      return nullptr != element_;
    }

   private:

    const Element *element_;

  };

  Deque();

  virtual ~Deque();

  void PushFront(Element *item);

  void PushBack(Element *item);

  void Insert(Element *item, int index = 0);

  Element *Remove(Element *item);

  size_t GetSize() const;

  bool IsEmpty() const;

  void Clear();

  Element *operator[](int index) const;

  Iterator begin() const {
    return Iterator(first_.next_);
  }

  ConstIterator cbegin() const {
    return ConstIterator(first_.next_);
  }

  Iterator rbegin() const {
    return Iterator(last_.previous_);
  }

  ConstIterator crbegin() const {
    return ConstIterator(last_.previous_);
  }

  Iterator end() const {
    return Iterator(const_cast<Element *>(&last_));
  }

  ConstIterator cend() const {
    return ConstIterator(&last_);
  }

  Iterator rend() const {
    return Iterator(const_cast<Element *>(&first_));
  }

  ConstIterator crend() const {
    return ConstIterator(&first_);
  }

 protected:

  const Element *first() const { return &first_; }

  const Element *last() const { return &last_; }

 private:

  Element first_;
  Element last_;

};

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_DEQUE_HPP_
