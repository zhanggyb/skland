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

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/path.hpp>
#include "skland/graphic/image-info.hpp"

#include "internal/matrix_private.hpp"
#include "internal/bitmap_private.hpp"
#include "internal/surface_private.hpp"
#include "internal/image-info_private.hpp"

#include "SkCanvas.h"

namespace skland {
namespace graphic {

using core::RectF;
using core::ColorF;

struct Canvas::Private {

  Private()
      : sk_canvas(nullptr) {}

  Private(const SkBitmap &bitmap)
      : sk_canvas(nullptr) {
    sk_canvas = new SkCanvas(bitmap);
  }

  ~Private() {
    delete sk_canvas;
  }

  SkCanvas *sk_canvas;
  Point2F origin;

};

Canvas *Canvas::CreateRasterDirect(int width, int height, unsigned char *pixels, int format) {
  size_t stride = (size_t) width * 4;

  // TODO: support more pixel format
  switch (format) {
    case kPixelFormatABGR8888:
    default:break;
  }

  Bitmap bitmap;
  if (!bitmap.InstallPixels(ImageInfo::MakeN32Premul(width, height), pixels, stride)) {
    throw std::runtime_error("ERROR! Invalid bitmap format for Canvas!");
  }

  return new Canvas(bitmap);
}

Canvas::Canvas() {
  p_.reset(new Private);
}

Canvas::Canvas(unsigned char *pixel, int width, int height, int format) {
  size_t stride = (size_t) width * 4;

  // TODO: support more pixel format
  switch (format) {
    case kPixelFormatABGR8888:
    default:break;
  }

  SkBitmap bitmap;
  if (!bitmap.installPixels(SkImageInfo::MakeN32Premul(width, height), pixel, stride)) {
    throw std::runtime_error("ERROR! Invalid bitmap format for Canvas!");
  }

  p_.reset(new Private(bitmap));
}

Canvas::Canvas(const Bitmap &bitmap) {
  p_.reset(new Private(bitmap.p_->sk_bitmap));
}

Canvas::~Canvas() {
}

void Canvas::SetOrigin(float x, float y) {
  p_->sk_canvas->translate(x - p_->origin.x, y - p_->origin.y);
  p_->origin.x = x;
  p_->origin.y = y;
}

Surface *Canvas::CreateSurface(const ImageInfo &info) {
  sk_sp<SkSurface> native = p_->sk_canvas->makeSurface(info.p_->sk_image_info);
  // TODO: create surface
  return nullptr;
}

void Canvas::DrawLine(float x0, float y0, float x1, float y1, const Paint &paint) {
  p_->sk_canvas->drawLine(x0, y0, x1, y1, paint.GetSkPaint());
}

void Canvas::DrawRect(const RectF &rect, const Paint &paint) {
  p_->sk_canvas->drawRect(*reinterpret_cast<const SkRect *>(&rect), paint.GetSkPaint());
}

void Canvas::DrawRoundRect(const RectF &rect, float rx, float ry, const Paint &paint) {
  p_->sk_canvas->drawRoundRect(*reinterpret_cast<const SkRect *>(&rect), rx, ry, paint.GetSkPaint());
}

void Canvas::DrawCircle(float x, float y, float radius, const Paint &paint) {
  p_->sk_canvas->drawCircle(x, y, radius, paint.GetSkPaint());
}

void Canvas::DrawArc(const RectF &oval, float start_angle, float sweep_angle, bool use_center, const Paint &paint) {
  p_->sk_canvas->drawArc(*reinterpret_cast<const SkRect *>(&oval),
                         start_angle,
                         sweep_angle,
                         use_center,
                         paint.GetSkPaint());
}

void Canvas::DrawPath(const Path &path, const Paint &paint) {
  p_->sk_canvas->drawPath(path.GetSkPath(), paint.GetSkPaint());
}

void Canvas::DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint) {
  p_->sk_canvas->drawText(text, byte_length, x, y, paint.GetSkPaint());
}

void Canvas::Translate(float dx, float dy) {
  p_->sk_canvas->translate(dx, dy);
}

void Canvas::Scale(float sx, float sy) {
  p_->sk_canvas->scale(sx, sy);
}

void Canvas::Rotate(float degrees) {
  p_->sk_canvas->rotate(degrees);
}

void Canvas::Rotate(float degrees, float px, float py) {
  p_->sk_canvas->rotate(degrees, px, py);
}

void Canvas::Skew(float sx, float sy) {
  p_->sk_canvas->skew(sx, sy);
}

void Canvas::Concat(const Matrix &matrix) {
  p_->sk_canvas->concat(matrix.p_->sk_matrix);
}

void Canvas::SetMatrix(const Matrix &matrix) {
  p_->sk_canvas->setMatrix(matrix.p_->sk_matrix);
}

void Canvas::ResetMatrix() {
  p_->sk_canvas->resetMatrix();
  p_->sk_canvas->translate(p_->origin.x, p_->origin.y);
}

void Canvas::Clear(uint32_t color) {
  p_->sk_canvas->clear(color);
}

void Canvas::Clear(const ColorF &color) {
  p_->sk_canvas->clear(color.argb());
}

void Canvas::ClipRect(const RectF &rect, ClipOperation op, bool antialias) {
  p_->sk_canvas->clipRect(reinterpret_cast<const SkRect &>(rect), static_cast<SkClipOp >(op), antialias);
}

void Canvas::ClipRect(const RectF &rect, bool antialias) {
  p_->sk_canvas->clipRect(reinterpret_cast<const SkRect &>(rect), antialias);
}

void Canvas::ClipPath(const Path &path, ClipOperation op, bool antialias) {
  p_->sk_canvas->clipPath(path.GetSkPath(), static_cast<SkClipOp >(op), antialias);
}

void Canvas::ClipPath(const Path &path, bool antilias) {
  p_->sk_canvas->clipPath(path.GetSkPath(), antilias);
}

void Canvas::Save() {
  p_->sk_canvas->save();
}

void Canvas::Restore() {
  p_->sk_canvas->restore();
}

int Canvas::GetSaveCount() const {
  return p_->sk_canvas->getSaveCount();
}

void Canvas::RestoreToCount(int save_count) {
  p_->sk_canvas->restoreToCount(save_count);
}

void Canvas::Flush() {
  p_->sk_canvas->flush();
}

const Point2F &Canvas::GetOrigin() const {
  return p_->origin;
}

SkCanvas *Canvas::GetSkCanvas() const {
  return p_->sk_canvas;
}

} // namespace graphic
} // namespace skland
