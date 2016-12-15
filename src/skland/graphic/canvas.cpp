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

Canvas::Canvas(unsigned char *pixel, int width, int height, int format)
    : sk_canvas_(nullptr) {
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

  sk_canvas_ = new SkCanvas(bitmap);
}

Canvas::~Canvas() {
  if (sk_canvas_) delete sk_canvas_;
}

void Canvas::DrawRectangle(float x, float y, float width, float height, const Paint &paint) {
  sk_canvas_->drawRect(SkRect::MakeXYWH(x, y, width, height), *paint.sk_paint());
}

void Canvas::DrawRect(const Rect &rect, const Paint &paint) {
  sk_canvas_->drawRect(*reinterpret_cast<const SkRect *>(&rect), *paint.sk_paint());
}

void Canvas::DrawRoundRect(const Rect &rect, float rx, float ry, const Paint &paint) {
  sk_canvas_->drawRoundRect(*reinterpret_cast<const SkRect *>(&rect), rx, ry, *paint.sk_paint());
}

void Canvas::DrawCircle(float x, float y, float radius, const Paint &paint) {
  sk_canvas_->drawCircle(x, y, radius, *paint.sk_paint());
}

void Canvas::DrawPath(const Path &path, const Paint &paint) {
  sk_canvas_->drawPath(*path.sk_path(), *paint.sk_paint());
}

void Canvas::DrawText(const void *text, size_t byte_length, float x, float y, const Paint &paint) {
  sk_canvas_->drawText(text, byte_length, x, y, *paint.sk_paint());
}

void Canvas::Translate(float dx, float dy) {
  sk_canvas_->translate(dx, dy);
}

void Canvas::ResetMatrix() {
  sk_canvas_->resetMatrix();
}

void Canvas::Clear(uint32_t color) {
  sk_canvas_->clear(color);
}

void Canvas::Clear(const Color &color) {
  sk_canvas_->clear(color.argb());
}

void Canvas::Clip(const Path &path, bool antilias) {
  sk_canvas_->clipPath(*path.sk_path(), antilias);
}

void Canvas::Save() {
  sk_canvas_->save();
}

void Canvas::Flush() {
  sk_canvas_->flush();
}

}
