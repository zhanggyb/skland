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

#include <memory>
#include <atomic>

namespace skland {
namespace core {

/**
 * @ingroup core
 * @brief The base class for objects that may be shared by multiple objects.
 * @tparam T The type of reference count, must be int, int32_t, long, size_t etc.
 */
template<typename T = int32_t>
class SPCountedBase {

  template<typename R> friend
  class SharedPtr;

  template<typename R> friend
  class WeekPtr;

 public:

  SPCountedBase(const SPCountedBase &) = delete;
  SPCountedBase(SPCountedBase &&) = delete;
  SPCountedBase &operator=(const SPCountedBase &) = delete;
  SPCountedBase &operator=(SPCountedBase &&) = delete;

  SPCountedBase()
      : reference_count_(1) {}

  virtual ~SPCountedBase() = default;

  T reference_count() const {
    return reference_count_.load(std::memory_order_relaxed);
  }

  bool unique() const {
    return 1 == reference_count();
  }

 private:

  std::atomic<T> reference_count_;

};

template<typename T>
class SharedPtr {

 public:

  SharedPtr() : ptr_(nullptr) {}

  explicit SharedPtr(T *obj) : ptr_(obj) {
    // TODO: fetch and add refcount
  }

 private:

  T *ptr_;

};

template<typename T>
class WeekPtr {

 public:

  WeekPtr() = default;

  WeekPtr(const WeekPtr &other);

  WeekPtr(const SharedPtr<T> &shared_ptr);

 private:

  SharedPtr<T> shared_ptr_;

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

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_MEMORY_HPP_
