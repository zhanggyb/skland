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

#ifndef SKLAND_CORE_MEMORY_HPP_
#define SKLAND_CORE_MEMORY_HPP_

#include "defines.hpp"

#include <memory>
#include <atomic>

namespace skland {
namespace core {

/**
 * @ingroup core
 * @brief The base class for objects that may be shared by multiple objects.
 * @tparam T The type of reference count, must be int, int32_t, long, size_t etc.
 *
 * Though we can use std::shared_ptr to store any type of object, there still be situation
 * that we need to indicate what kind of object can be used in shared ptr.
 */
class SPCountedBase {

  template<typename T> friend
  class SharedPtr;

  template<typename T> friend
  class WeakPtr;

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(SPCountedBase);

  struct Counter {

    Counter()
        : use_count(0), weak_count(0) {
      _DEBUG("%s\n", __func__);
    }

    ~Counter() {
      _DEBUG("%s\n", __func__);
    }

    /**
     * @brief The number of SharedPtr instances pointing to the object
     */
    std::atomic_ulong use_count;

    /**
     * @brief The number of WeakPtr instances pointing to the object,
     * plus one if the "use_count" is still > 0
     */
    std::atomic_ulong weak_count;

  };

  SPCountedBase()
      : counter_(new Counter) {}

  virtual ~SPCountedBase() {
    if (nullptr != counter_) {
      _DEBUG("use_count: %ld, weak_count: %ld\n", counter_->use_count.load(), counter_->weak_count.load());
    }
    delete counter_;
  }

  size_t use_count() const {
    return counter_->use_count.load();
  }

  size_t weak_count() const {
    return counter_->weak_count.load();
  }
  
 private:

  Counter *counter_;

};

/**
 * @ingroup core
 * @brief Shared pointer
 * @tparam T
 */
template<typename T>
class SharedPtr {

  template<typename R> friend
  class WeakPtr;

  template<typename R>
  friend void swap(SharedPtr<R> &src, SharedPtr<R> &dst);

 public:

  SharedPtr() = default;

  explicit SharedPtr(T *obj) : ptr_(obj) {
    counter_ = ptr_->counter_;
    ++counter_->use_count;
  }

  SharedPtr(const SharedPtr &orig)
      : ptr_(orig.ptr_) {
    if (nullptr != ptr_) {
      counter_ = ptr_->counter_;
      ++counter_->use_count;
    }
  }

  /**
   * @brief Move constructor
   * @param orig
   */
  SharedPtr(SharedPtr &&orig) noexcept
      : ptr_(orig.ptr_), counter_(orig.counter_) {
    orig.ptr_ = nullptr;
    orig.counter_ = nullptr;
  }

  ~SharedPtr() {
    if (nullptr != counter_) {
      _ASSERT((nullptr != ptr_) && (ptr_->counter_ == counter_));

      --counter_->use_count;
      if (0 == counter_->use_count) {
        ptr_->counter_ = nullptr; // set this value to avoid destroying the counter

        // Important: avoid double delete counter_:
        ++counter_->weak_count;
        delete ptr_;
        --counter_->weak_count;

        if (0 == counter_->weak_count) delete counter_;
      }
    } else {
      _ASSERT(nullptr == ptr_);
    }
  }

  /**
   * @brief Copy assignment
   * @param other
   * @return
   */
  SharedPtr &operator=(const SharedPtr &other) noexcept {
    reset(other.ptr_);
    return *this;
  }

  /**
   * @brief Move assignment
   * @param other
   * @return
   */
  SharedPtr &operator=(SharedPtr &&other) noexcept {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
    return *this;
  }

  SharedPtr &operator=(T *obj) noexcept {
    reset(obj);
    return *this;
  }

  /**
   * @brief Swap content
   * @param other
   */
  void swap(SharedPtr &other) noexcept {
    T *object = ptr_;
    SPCountedBase::Counter *counter = counter_;

    ptr_ = other.ptr_;
    counter_ = other.counter_;

    other.ptr_ = object;
    other.counter_ = counter;
  }

  /**
   * @brief Reset pointer
   * @param obj
   */
  void reset(T *obj = nullptr) {
    T *old_object = ptr_;
    SPCountedBase::Counter *old_counter = counter_;

    ptr_ = obj;
    counter_ = nullptr;
    if (nullptr != ptr_) {
      counter_ = ptr_->counter_;
      ++counter_->use_count;
    }

    if (nullptr != old_counter) {
      _ASSERT((nullptr != old_object) && (old_object->counter_ == old_counter));

      --old_counter->use_count;
      if (0 == old_counter->use_count) {
        old_object->counter_ = nullptr;

        // Important: avoid double delete old_counter:
        ++old_counter->weak_count;
        delete old_object;
        --old_counter->weak_count;

        if (0 == old_counter->weak_count) delete old_counter;
      }
    } else {
      _ASSERT(nullptr == old_object);
    }
  }

  /**
   * @brief Get pointer
   * @return
   */
  T *get() const {
    return ptr_;
  }

  /**
   * @brief Dereference object
   * @return
   */
  T &operator*() const {
    return *ptr_;
  }

  /**
   * @brief Dereference object member
   * @return
   */
  T *operator->() const {
    return ptr_;
  }

  /**
   * @brief Use count
   * @return
   */
  size_t use_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->use_count.load();
  }

  size_t weak_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->weak_count.load();
  }

  /**
   * @brief Returns whether this shared pointer does not share ownership over its pointer with other.
   * @return
   */
  bool unique() const noexcept {
    return 1 == use_count();
  }

  explicit operator bool() const {
    if (nullptr == ptr_) {
      _ASSERT(nullptr == counter_);
      return false;
    }

    _ASSERT(ptr_->counter_ == counter_);
    return true;
  }

 private:

  T *ptr_ = nullptr;

  SPCountedBase::Counter *counter_ = nullptr;

};

template<typename T>
class WeakPtr {

 public:

  WeakPtr() = default;

  WeakPtr(const WeakPtr &orig) {
    ptr_ = orig.ptr_;
    counter_ = orig.counter_;
    if (nullptr != counter_) ++counter_->weak_count;
  }

  explicit WeakPtr(const SharedPtr<T> &orig) {
    ptr_ = orig.ptr_;
    counter_ = orig.counter_;
    if (nullptr != counter_) ++counter_->weak_count;
  }

  ~WeakPtr() {
    if (nullptr != counter_) {
      --counter_->weak_count;
      if ((0 == counter_->use_count) && (0 == counter_->weak_count)) {
        delete counter_;
      }
    }
  }

  WeakPtr &operator=(const SharedPtr<T> &other) {
    SPCountedBase::Counter *old_counter = counter_;

    ptr_ = other.ptr_;
    counter_ = other.counter_;

    if (nullptr != counter_) ++counter_->weak_count;

    if (nullptr != old_counter) {
      --old_counter->weak_count;
      if ((0 == old_counter->use_count) && (0 == old_counter->weak_count)) {
        delete old_counter;
      }
    }

    return *this;
  }

  WeakPtr &operator=(const WeakPtr &other) {
    SPCountedBase::Counter *old_counter = counter_;

    ptr_ = other.ptr_;
    counter_ = other.counter_;

    if (nullptr != counter_) ++counter_->weak_count;

    if (nullptr != old_counter) {
      --old_counter->weak_count;
      if ((0 == old_counter->use_count) && (0 == old_counter->weak_count)) {
        delete old_counter;
      }
    }

    return *this;
  }

  void reset() {
    ptr_ = nullptr;
    if (nullptr != counter_) {
      --counter_->weak_count;
      if ((0 == counter_->use_count) && (0 == counter_->weak_count)) {
        delete counter_;
      }
      counter_ = nullptr;
    }
  }

  size_t use_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->use_count.load();
  }

  size_t weak_count() const noexcept {
    return nullptr == counter_ ? 0 : counter_->weak_count.load();
  }

  SharedPtr<T> lock() const noexcept {
    return SharedPtr<T>(ptr_);
  }

 private:

  T *ptr_ = nullptr;

  SPCountedBase::Counter *counter_ = nullptr;

};

/**
 * @ingroup core
 * @brief Constructs an object of type T and wraps it in a std::unique_ptr
 * @tparam T
 * @tparam Args
 * @param args list of arguments with which an instance of T will be constructed
 * @return std::unique_ptr of an instance of type T
 *
 * This template function is the same as std::make_unique in C++14
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename ... Args>
SharedPtr<T> make_shared(Args &&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
};

template<typename T>
void swap(SharedPtr<T> &src, SharedPtr<T> &dst) {
  T *object = src.ptr_;
  SPCountedBase::Counter *counter = src.counter_;

  src.ptr_ = dst.ptr_;
  src.counter_ = dst.counter_;

  dst.ptr_ = object;
  dst.counter_ = counter;
}

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_MEMORY_HPP_
