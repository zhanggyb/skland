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

#ifndef SKLAND_GRAPHIC_INTERNAL_LAYER_PRIVATE_HPP_
#define SKLAND_GRAPHIC_INTERNAL_LAYER_PRIVATE_HPP_

#include "skland/graphic/layer.hpp"

#include "SkSurface.h"

namespace skland {
namespace graphic {

struct Surface::Private {

  Private() {}

  Private(sk_sp<SkSurface> native)
      : sp_sk_surface(native) {}

  Private(const Private &orig)
      : sp_sk_surface(orig.sp_sk_surface) {}

  Private &operator=(const Private &other) {
    sp_sk_surface = other.sp_sk_surface;
    return *this;
  }

  ~Private() {}

  sk_sp<SkSurface> sp_sk_surface;

};

} // namespace graphic
} // namespace skland

#endif // SKLAND_GRAPHIC_INTERNAL_LAYER_PRIVATE_HPP_
