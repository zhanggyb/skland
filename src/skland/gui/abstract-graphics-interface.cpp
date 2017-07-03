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

#include "internal/abstract-graphics-interface_private.hpp"

#include "skland/core/assert.hpp"

namespace skland {
namespace gui {

AbstractGraphicsInterface::AbstractGraphicsInterface() {
  p_.reset(new Private);
}

AbstractGraphicsInterface::~AbstractGraphicsInterface() {
  if (p_->surface) {
    _ASSERT(p_->surface->p_->graphics_interface == this);
    p_->surface->p_->graphics_interface = nullptr;
  }
}

Surface* AbstractGraphicsInterface::GetSurface() const {
  return p_->surface;
}

} // namespace gui
} // namespace skland
