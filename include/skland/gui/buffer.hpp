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

#include "skland/core/point.hpp"
#include "skland/core/size.hpp"
#include "skland/core/sigcxx.hpp"
#include "skland/core/defines.hpp"

#include <cstdint>
#include <memory>

namespace skland {
namespace gui {

class SharedMemoryPool;

/**
 * @brief Buffer in wayland compositor
 *
 * This class represents a native wayland buffer in memory. Use the Setup() to
 * allocate this buffer and should allocate again if the shm pool is reset.
 */
class Buffer {

  friend class Surface;

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Buffer);

  using Point = core::PointI;
  using Size = core::SizeI;

  template<typename ... ParamTypes>
  using SignalRef = typename core::SignalRef<ParamTypes...>;

  template<typename ... ParamTypes>
  using Signal = typename core::Signal<ParamTypes...>;

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

  const void *GetData() const;

  int32_t GetStride() const;

  int GetOffset() const;

  const Point &GetPosition() const;

  const Size &GetSize() const;

  SignalRef<> release() {
    return release_;
  }

  SignalRef<> destroyed() {
    return destroyed_;
  }

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  Signal<> release_;

  Signal<> destroyed_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_BUFFER_HPP_
