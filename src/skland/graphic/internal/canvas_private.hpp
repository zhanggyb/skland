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

#ifndef SKLAND_GRAPHIC_INTERNAL_CANVAS_PRIVATE_HPP_
#define SKLAND_GRAPHIC_INTERNAL_CANVAS_PRIVATE_HPP_

#include "skland/graphic/canvas.hpp"

#include "bitmap_private.hpp"

#include "skland/core/deque.hpp"

#include "SkCanvas.h"

namespace skland {
namespace graphic {

/**
 * @ingroup graphic_intern
 * @brief The private structure used in Canvas
 */
struct Canvas::Private {

  Private() = default;

  explicit Private(const SkBitmap &bitmap)
      : sk_canvas(bitmap) {
  }

  ~Private() = default;

  SkCanvas sk_canvas;

  core::PointF origin;

  size_t lock_count = 0;

  core::Deque<LockGuardNode> lock_guard_deque;

};

} // namespace graphic
} // namespace skland

#endif // SKLAND_GRAPHIC_INTERNAL_CANVAS_PRIVATE_HPP_
