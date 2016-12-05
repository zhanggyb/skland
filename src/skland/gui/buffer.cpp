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

#include <skland/gui/buffer.hpp>
#include <skland/gui/memory-pool.hpp>

namespace skland {

void Buffer::Setup(const MemoryPool &pool,
                   int32_t width,
                   int32_t height,
                   int32_t stride,
                   uint32_t format,
                   int offset) {
  Destroy();

  int size = stride * height;
  if ((offset + size) > pool.size()) {
    throw std::runtime_error("Error! Trying to allocate buffer on small SHM pool.");
  }

  wl_buffer_.Setup(pool.wl_shm_pool(), offset, width,
                   height, stride, format);
  size_.width = width;
  size_.height = height;
  stride_ = stride;
  format_ = format;
  data_ = pool.data();
  pixel_ = (char *) data_->data() + offset;
}

void Buffer::Destroy() {
  if (wl_buffer_.IsValid()) {
    data_.reset();
    pixel_ = nullptr;
    format_ = 0;
    stride_ = 0;
    size_.width = 0;
    size_.height = 0;
    wl_buffer_.Destroy();
  }
}

}
