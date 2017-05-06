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

#include "../core/point.hpp"
#include "../core/size.hpp"
#include "../core/delegate.hpp"

#include <wayland-client.h>

#include <cstdint>
#include <memory>

namespace skland {

namespace wayland {
class Buffer;
}

class SharedMemoryPool;

/**
 * @brief Buffer in wayland compositor
 *
 * This class represents a native wayland buffer in memory. Use the Setup() to
 * allocate this buffer and should allocate again if the shm pool is reset.
 */
class Buffer {

  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

 public:

  Buffer();

  ~Buffer();

  void Setup(const SharedMemoryPool &pool,
             int32_t width,
             int32_t height,
             int32_t stride,
             uint32_t format,
             int offset = 0);

  void Destroy();

  void SetPosition(int x, int y);

  const wayland::Buffer &wl_buffer() const;

  const void *GetData() const;

  int32_t GetStride() const;

  int GetOffset() const;

  const Point &GetPosition() const;

  const Size &GetSize() const;

  DelegateRef<void()> release() {
    return release_;
  }

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  static void OnRelease(void *data, struct wl_buffer *buffer);

  static const struct wl_buffer_listener kListener;

  Delegate<void()> release_;

};

}

#endif // SKLAND_GUI_BUFFER_HPP_
