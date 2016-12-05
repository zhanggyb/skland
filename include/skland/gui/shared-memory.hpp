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

#ifndef SKLAND_GUI_SHARED_MEMORY_HPP_
#define SKLAND_GUI_SHARED_MEMORY_HPP_

#include <sys/mman.h>
#include <memory>

namespace skland {

/**
 * @brief Share memory
 *
 * This class represents a block of shared memory. It's usually created in
 * SHMPool and stored in a std::shared_ptr -- this let the SHMPool and Buffer
 * share the same memory and release it when it's not needed by either of them.
 */
class SharedMemory {

  SharedMemory() = delete;
  SharedMemory(const SharedMemory &) = delete;
  SharedMemory &operator=(const SharedMemory &) = delete;

 public:

  typedef std::shared_ptr<SharedMemory> SharedPtr;

  SharedMemory(int fd, size_t size);

  ~SharedMemory();

  size_t size() const {
    return size_;
  }

  void *data() const {
    return data_;
  }

 private:

  void *data_;
  size_t size_;

};

}

#endif // SKLAND_GUI_SHARED_MEMORY_HPP_
