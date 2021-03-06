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

#include "skland/core/types.hpp"
#include "skland/core/size.hpp"
#include "skland/core/rect.hpp"
#include "skland/core/color.hpp"
#include "skland/core/defines.hpp"
#include "skland/core/deque.hpp"

#include "clip-operation.hpp"

#include <memory>

// Forward declaration
class SkCanvas;

namespace skland {
namespace graphic {

// Forward declarations
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

  friend class Surface;

 public:

  using RectF = core::RectF;

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Canvas);

  class LockGuard;

  static Canvas *CreateRasterDirect(int width,
                                    int height,
                                    unsigned char *pixels,
                                    int format = kPixelFormatABGR8888);

  /**
   * @brief Create an empty canvas with no backing device/pixels, and zero dimentions
   */
  Canvas();

  Canvas(unsigned char *pixel, int width, int height,
         int format = kPixelFormatABGR8888);

  explicit Canvas(const Bitmap &bitmap);

  ~Canvas();

  void SetOrigin(float x, float y);

  /**
   * @brief Create a new surface matching the specified info
   * @param info
   * @return A pointer to a new created surface object, must be deleted manually
   */
  Surface *CreateSurface(const ImageInfo &info);

  void DrawLine(float x0, float y0, float x1, float y1, const Paint &paint);

  void DrawRect(const RectF &rect, const Paint &paint);

  void DrawRoundRect(const RectF &rect, float rx, float ry, const Paint &paint);

  void DrawOval(const RectF &oval, const Paint &paint);

  void DrawCircle(float x, float y, float radius, const Paint &paint);

  void DrawArc(const RectF &oval, float start_angle, float sweep_angle,
               bool use_center, const Paint &paint);

  void DrawPath(const Path &path, const Paint &paint);

  void DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint);

  void DrawPaint(const Paint &paint);

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

  void Clear(uint32_t argb = 0x0);

  void Clear(const core::ColorF &color);

  void ClipRect(const RectF &rect, ClipOperation op, bool antialias = false);

  void ClipRect(const RectF &rect, bool antialias = false);

  void ClipPath(const Path &path, ClipOperation op, bool antialias = false);

  void ClipPath(const Path &path, bool antialias = false);

  void Save();

  /**
   * @brief Save clip and matrix on stack, creates layer
   * @param bounds Hint to limit the size of the layer, may be nullptr
   * @param paint Graphics state for layer, may be nullptr
   */
  void SaveLayer(const RectF *bounds, const Paint *paint);

  /**
   * @brief Save clip and matrix on stack, create layer
   * @param bounds
   * @param alpha
   */
  void SaveLayer(const RectF *bounds, unsigned char alpha);

  void Restore();

  int GetSaveCount() const;

  void RestoreToCount(int save_count);

  void Flush();

  const core::PointF &GetOrigin() const;

  SkCanvas *GetSkCanvas() const;

 private:

  struct LockGuardNode;
  struct Private;

  std::unique_ptr<Private> p_;

};

struct Canvas::LockGuardNode : public core::BiNode {

  LockGuardNode() = default;
  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(LockGuardNode);

  ~LockGuardNode() final = default;

  int depth = 0;

};

class Canvas::LockGuard {

 public:

  LockGuard() = delete;
  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(LockGuard);

  explicit LockGuard(Canvas *canvas)
      : canvas_(canvas) {}

  LockGuard(Canvas *canvas, const RectF &rect, bool antialias = false)
      : canvas_(canvas) {
    Lock(rect, antialias);
  }

  LockGuard(Canvas *canvas, const RectF &rect, ClipOperation op, bool antialias = false)
      : canvas_(canvas) {
    Lock(rect, op, antialias);
  }

  LockGuard(Canvas *canvas, const Path &path, bool antialias = false)
      : canvas_(canvas) {
    Lock(path, antialias);
  }

  LockGuard(Canvas *canvas, const Path &path, ClipOperation op, bool antialias = false)
      : canvas_(canvas) {
    Lock(path, op, antialias);
  }

  ~LockGuard();

  void Lock(const RectF &rect, bool antilias = false);

  void Lock(const RectF &rect, ClipOperation op, bool antialias = false);

  void Lock(const Path &path, bool antialias = false);

  void Lock(const Path &path, ClipOperation op, bool antialias = false);

  void Unlock();

 private:

  Canvas *canvas_ = nullptr;

  LockGuardNode node_;

};

} // namespace graphic
} // namespace skland

#endif // SKLAND_GRAPHIC_CANVAS_HPP_
