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

  template<typename R> friend
  class SharedPtr;

 public:

  struct Counter {

    Counter()
        : use_count(1), weak_count(0) {}

    std::atomic_ulong use_count;
    std::atomic_ulong weak_count;

  };

  SPCountedBase(const SPCountedBase &) = delete;
  SPCountedBase(SPCountedBase &&) = delete;
  SPCountedBase &operator=(const SPCountedBase &) = delete;
  SPCountedBase &operator=(SPCountedBase &&) = delete;

  SPCountedBase()
      : counter_(new Counter) {}

  virtual ~SPCountedBase() {
    delete counter_;
  }

  size_t use_count() const {
    return counter_->use_count.load();
  }

  size_t weak_count() const {
    return counter_->weak_count.load();
  }

  bool unique() const {
    return 1 == counter_->use_count.load();
  }

 private:

  Counter *counter_;

};

template<typename T>
class SharedPtr {

  template<typename R> friend
  class WeekPtr;

 public:

  SharedPtr() = default;

  explicit SharedPtr(T *obj) : ptr_(obj) {
    if (nullptr != ptr_) {
      counter_ = ptr_->counter_;
      ++counter_->use_count;
    }
  }

  SharedPtr(const SharedPtr &orig)
      : ptr_(orig.ptr_) {
    if (nullptr != ptr_) {
      counter_ = ptr_->counter_;
      ++counter_->use_count;
    }
  }

  SharedPtr(SharedPtr &&orig) noexcept
      : ptr_(orig.ptr_), counter_(orig.counter_) {
    orig.ptr_ = nullptr;
    orig.counter_ = nullptr;
  }

  ~SharedPtr() {
    if (nullptr != ptr_) {
      // assert counter_
      if (1 == (--counter_->use_count)) {
        ptr_->counter_ = nullptr;
        delete ptr_;
      }
    }

    if (nullptr != counter_) {
      if ((1 == counter_->use_count) && (0 == counter_->weak_count))
        delete counter_;
    }
  }

  SharedPtr &operator=(const SharedPtr &other) noexcept {
    reset(other.ptr_);
    return *this;
  }

  SharedPtr &operator=(SharedPtr &&other) noexcept {
    ptr_ = other.ptr_;
    counter_ = other.counter_;
    other.ptr_ = nullptr;
    other.counter_ = nullptr;
    return *this;
  }

  void reset(T *obj) {
    T *old_object = ptr_;
    SPCountedBase::Counter *old_counter = counter_;
    ptr_ = obj;
    counter_ = obj->counter_;

    if (nullptr != ptr_) {
      ++counter_->use_count;
    }

    if (nullptr != old_object) {
      if (1 == (--old_counter->use_count)) {
        old_object->counter_ = nullptr;
        delete old_object;
      }
    }

    if (nullptr != old_counter) {
      if ((1 == old_counter->use_count) && (0 == old_counter->weak_count))
        delete old_counter;
    }
  }

  T *operator->() const {
    return ptr_;
  }

  T &operator*() const {
    return *ptr_;
  }

  T *get() const {
    return ptr_;
  }

  explicit operator bool() const {
    return nullptr != ptr_;
  }

 private:

  T *ptr_ = nullptr;

  SPCountedBase::Counter *counter_ = nullptr;

};

//template<typename T>
//class WeekPtr {
//
// public:
//
//  using element_type = T;
//
//  WeekPtr() = default;
//
//  ~WeekPtr() {
////    shared_ptr_.ptr_ = nullptr;
//  }
//
//  WeekPtr(const WeekPtr &other) {
////    shared_ptr_.ptr_ = other.shared_ptr_.ptr_;
//  }
//
//  explicit WeekPtr(const SharedPtr<T> &ptr) {
////    shared_ptr_.ptr_ = ptr->ptr_;
//  }
//
// private:
//
//  T *ptr_ = nullptr;
//
//  typename SPCountedBase<T>::Counter *counter_ = nullptr;
//
//};

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

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_MEMORY_HPP_
