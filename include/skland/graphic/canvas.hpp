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
#include "../core/color.hpp"

#include "clip-operation.hpp"

#include <memory>

class SkCanvas;

namespace skland {
namespace graphic {

class Paint;
class Path;
class Matrix;
class Bitmap;
class ImageInfo;
class Surface;

/**
 * @ingroup graphic
 * @brief Canvas for drawing
 */
class Canvas {

  Canvas(const Canvas &) = delete;
  Canvas &operator=(const Canvas &) = delete;

 public:

  class ClipGuard;

  static std::unique_ptr<Canvas> MakeRasterDirect(int width,
                                                  int height,
                                                  unsigned char *pixels,
                                                  int format = kPixelFormatABGR8888);

  /**
   * @brief Create an empty canvas with no backing device/pixels, and zero dimentions
   */
  Canvas();

  Canvas(unsigned char *pixel, int width, int height,
         int format = kPixelFormatABGR8888);

  Canvas(const Bitmap &bitmap);

  ~Canvas();

  void SetOrigin(float x, float y);

  /**
   * @brief Create a new layer matching the specified info
   * @param info
   * @return
   */
  Surface MakeLayer(const ImageInfo &info);

  void DrawLine(float x0, float y0, float x1, float y1, const Paint &paint);

  void DrawRect(const Rect &rect, const Paint &paint);

  void DrawRoundRect(const Rect &rect, float rx, float ry, const Paint &paint);

  void DrawCircle(float x, float y, float radius, const Paint &paint);

  void DrawArc(const Rect &oval, float start_angle, float sweep_angle,
               bool use_center, const Paint &paint);

  void DrawPath(const Path &path, const Paint &paint);

  void DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint);

  void Translate(float dx, float dy);

  void Scale(float sx, float sy);

  void Rotate(float degrees);

  void Rotate(float degrees, float px, float py);

  void Skew(float sx, float sy);

  void Concat(const Matrix &matrix);

  void SetMatrix(const Matrix &matrix);

  /**
   * @brief Reset matrix and translate to origin
   */
  void ResetMatrix();

  void Clear(uint32_t color = 0x0);

  void Clear(const Color &color);

  void ClipRect(const Rect &rect, ClipOperation op, bool antialias = false);

  void ClipRect(const Rect &rect, bool antialias = false);

  void ClipPath(const Path &path, ClipOperation op, bool antialias = false);

  void ClipPath(const Path &path, bool antialias = false);

  void Save();

  void Restore();

  int GetSaveCount() const;

  void RestoreToCount(int save_count);

  void Flush();

  const Point2F &GetOrigin() const;

  SkCanvas *GetSkCanvas() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

class Canvas::ClipGuard {

  ClipGuard() = delete;
  ClipGuard(const ClipGuard &) = delete;
  ClipGuard &operator=(const ClipGuard &) = delete;

 public:

  ClipGuard(Canvas *canvas, const Rect &rect, bool antialias = false)
      : canvas_(canvas) {
    canvas_->Save();
    canvas_->ClipRect(rect, antialias);
  }

  ClipGuard(Canvas *canvas, const Rect &rect, ClipOperation op, bool antialias = false)
      : canvas_(canvas) {
    canvas_->Save();
    canvas_->ClipRect(rect, op, antialias);
  }

  ClipGuard(Canvas *canvas, const Path &path, bool antialias = false)
      : canvas_(canvas) {
    canvas_->Save();
    canvas_->ClipPath(path, antialias);
  }

  ClipGuard(Canvas *canvas, const Path &path, ClipOperation op, bool antialias = false)
      : canvas_(canvas) {
    canvas_->Save();
    canvas_->ClipPath(path, op, antialias);
  }

  ~ClipGuard() {
    canvas_->Restore();
  }

 private:

  Canvas *canvas_;

};

} // namespace graphic
} // namespace skland

#endif // SKLAND_GRAPHIC_CANVAS_HPP_
