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

#include "SkBitmap.h"
#include "SkCanvas.h"

namespace skland {

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

  metadata_.reset(new SkCanvas(bitmap));
}

Canvas::~Canvas() {
}

void Canvas::SetOrigin(float x, float y) {
  metadata_->translate(x - origin_.x, y - origin_.y);
  origin_.x = x;
  origin_.y = y;
}

void Canvas::DrawLine(float x0, float y0, float x1, float y1, const Paint &paint) {
  metadata_->drawLine(x0, y0, x1, y1, *paint.sk_paint());
}

void Canvas::DrawRect(const Rect &rect, const Paint &paint) {
  metadata_->drawRect(*reinterpret_cast<const SkRect *>(&rect), *paint.sk_paint());
}

void Canvas::DrawRoundRect(const Rect &rect, float rx, float ry, const Paint &paint) {
  metadata_->drawRoundRect(*reinterpret_cast<const SkRect *>(&rect), rx, ry, *paint.sk_paint());
}

void Canvas::DrawCircle(float x, float y, float radius, const Paint &paint) {
  metadata_->drawCircle(x, y, radius, *paint.sk_paint());
}

void Canvas::DrawPath(const Path &path, const Paint &paint) {
  metadata_->drawPath(*path.sk_path(), *paint.sk_paint());
}

void Canvas::DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint) {
  metadata_->drawText(text, byte_length, x, y, *paint.sk_paint());
}

void Canvas::Translate(float dx, float dy) {
  metadata_->translate(dx, dy);
}

void Canvas::ResetMatrix() {
  metadata_->resetMatrix();
  metadata_->translate(origin_.x, origin_.y);
}

void Canvas::Clear(uint32_t color) {
  metadata_->clear(color);
}

void Canvas::Clear(const Color &color) {
  metadata_->clear(color.argb());
}

void Canvas::ClipRect(const Rect &rect, bool antialias) {
  metadata_->clipRect(reinterpret_cast<const SkRect &>(rect), antialias);
}

void Canvas::ClipPath(const Path &path, bool antilias) {
  metadata_->clipPath(*path.sk_path(), antilias);
}

void Canvas::Save() {
  metadata_->save();
}

void Canvas::Restore() {
  metadata_->restore();
}

void Canvas::Flush() {
  metadata_->flush();
}

}
