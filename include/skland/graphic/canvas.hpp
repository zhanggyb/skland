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

#ifndef SKLAND_GRAPHIC_CANVAS_HPP_
#define SKLAND_GRAPHIC_CANVAS_HPP_

#include "../core/types.hpp"
#include "../core/margin.hpp"
#include "../core/size.hpp"
#include "../core/rect.hpp"

#include <memory>
#include <skland/core/color.hpp>

class SkCanvas;

namespace skland {

class Paint;
class Path;

class Canvas {

  Canvas(const Canvas &) = delete;
  Canvas &operator=(const Canvas &) = delete;

 public:

  Canvas(const Margin &margin = Margin())
      : sk_canvas_(nullptr), margin_(margin) {
  }

  ~Canvas();

  void Setup(unsigned char *pixel, int width, int height,
             int format = kPixelFormatABGR8888);

  void Destroy();

  void DrawRectangle(float x, float y, float width, float height, const Paint &paint) const;

  void DrawRect(const RectF &rect, const Paint &paint) const;

  void DrawRoundRect(const Rect &rect, float rx, float ry, const Paint &paint) const;

  void DrawRoundRect(const RectF &rect, float rx, float ry, const Paint &paint) const;

  void DrawCircle(float x, float y, float radius, const Paint &paint) const;

  void DrawPath(const Path &path, const Paint &paint) const;

  void DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint) const;

  void Translate(float dx, float dy) const;

  void ResetMatrix() const;

  void Clear(uint32_t color = 0x0) const;

  void Clear(const Color &color) const;

  void Save() const;

  void Flush() const;

  void SetMargin(const Margin &margin = Margin());

  const Margin &margin() const {
    return margin_;
  }

  bool is_valid() const {
    return sk_canvas_ != nullptr;
  }

  bool is_null() const {
    return sk_canvas_ == nullptr;
  }

  SkCanvas *sk_canvas() const {
    return sk_canvas_;
  }

 private:

  SkCanvas *sk_canvas_;

  Margin margin_;

};

}

#endif // SKLAND_GRAPHIC_CANVAS_HPP_
