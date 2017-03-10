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
#include <skland/gui/shared-memory-pool.hpp>

#include <skland/wayland/buffer.hpp>

namespace skland {

struct Buffer::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : stride(0),
        format(0),
        offset(0),
        data(nullptr) {}

  ~Private() {}

  wayland::Buffer wl_buffer;

  /**
   * @brief Position on surface
   */
  Point position;

  Size size;

  int32_t stride;

  uint32_t format;

  int offset;

  void *data;

};

Buffer::Buffer() {
  p_.reset(new Private);
}

Buffer::~Buffer() {

}

void Buffer::Setup(const SharedMemoryPool &pool,
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

  p_->wl_buffer.Setup(pool.wl_shm_pool(), offset, width,
                      height, stride, format);
  p_->size.width = width;
  p_->size.height = height;
  p_->stride = stride;
  p_->format = format;
  p_->offset = offset;
  p_->data = (char *) pool.data() + offset;
}

void Buffer::Destroy() {
  if (p_->wl_buffer.IsValid()) {
    p_->data = nullptr;
    p_->offset = 0;
    p_->format = 0;
    p_->stride = 0;
    p_->size.width = 0;
    p_->size.height = 0;
    p_->wl_buffer.Destroy();
  }
}

void Buffer::SetPosition(int x, int y) {
  p_->position.x = x;
  p_->position.y = y;
}

const wayland::Buffer &Buffer::wl_buffer() const {
  return p_->wl_buffer;
}

const void *Buffer::GetData() const {
  return p_->data;
}

int32_t Buffer::GetStride() const {
  return p_->stride;
}

int Buffer::GetOffset() const {
  return p_->offset;
}

const Point &Buffer::GetPosition() const {
  return p_->position;
}

const Size &Buffer::GetSize() const {
  return p_->size;
}

DelegateRef<void()> Buffer::release() {
  return p_->wl_buffer.release();
}

}
