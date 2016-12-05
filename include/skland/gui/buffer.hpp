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

#ifndef SKLAND_GUI_BUFFER_HPP_
#define SKLAND_GUI_BUFFER_HPP_

#include "shared-memory.hpp"

#include <skland/core/size.hpp>
#include <skland/wayland/client/buffer.hpp>

namespace skland {

class MemoryPool;

/**
 * @brief Buffer in wayland compositor
 *
 * This class represents a buffer in memory. It's allocated by SHMPool and holds
 * a std::shared_ptr points to the shared memory.
 */
class Buffer {

  friend class MemoryPool;

  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

 public:

  Buffer()
      : stride_(0),
        format_(0),
        pixel_(nullptr) {
  }

  ~Buffer() {}

  void Setup(const MemoryPool &pool,
             int32_t width,
             int32_t height,
             int32_t stride,
             uint32_t format,
             int offset = 0);

  void Destroy();

  const wayland::client::Buffer &wl_buffer() const {
    return wl_buffer_;
  }

  const void *pixel() const {
    return pixel_;
  }

  int32_t stride() const {
    return stride_;
  }

  const Size &size() const {
    return size_;
  }

  DelegateRef<void()> release() {
    return wl_buffer_.release();
  }

 private:

  wayland::client::Buffer wl_buffer_;

  Size size_;

  int32_t stride_;

  uint32_t format_;

  void *pixel_;

  SharedMemory::SharedPtr data_;

};

}

#endif // SKLAND_GUI_BUFFER_HPP_
