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

#include <skland/gui/shared-memory.hpp>
#include <skland/core/defines.hpp>

namespace skland {

SharedMemory::SharedMemory(int fd, size_t size)
    : data_(nullptr), size_(0) {
  data_ = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data_ == MAP_FAILED) {
    DBG_PRINT_MSG("%s\n", "Fail to map pages of memory");
    data_ = nullptr;
  } else {
    size_ = size;
  }
}

SharedMemory::~SharedMemory() {
  if (data_ && munmap(data_, size_))
    DBG_PRINT_MSG("%s\n", "Failed to unmap the memory\n");
}

}
