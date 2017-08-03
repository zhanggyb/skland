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

#include "internal/abstract-gpu-interface_private.hpp"

#include "skland/core/assert.hpp"
#include "skland/core/memory.hpp"

namespace skland {
namespace gui {

AbstractGPUInterface::AbstractGPUInterface() {
  p_ = core::make_unique<Private>();
}

AbstractGPUInterface::~AbstractGPUInterface() {
  if (nullptr != p_->surface) {
    _ASSERT(p_->surface->p_->gpu_interface == this);
    p_->surface->p_->gpu_interface = nullptr;
  }
}

Surface *AbstractGPUInterface::GetSurface() const {
  return p_->surface;
}

} // namespace gui
} // namespace skland
