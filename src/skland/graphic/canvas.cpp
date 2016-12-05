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

Canvas::~Canvas() {
  if (sk_canvas_) delete sk_canvas_;
}

void Canvas::Setup(unsigned char *pixel, int width, int height, int format) {
  Destroy();

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
  sk_canvas_->translate(margin_.left, margin_.top);
}

void Canvas::Destroy() {
  if (sk_canvas_) {
    delete sk_canvas_;
    sk_canvas_ = nullptr;
  }
}

void Canvas::DrawRectangle(float x, float y, float width, float height, const Paint &paint) const {
  sk_canvas_->drawRect(SkRect::MakeXYWH(x, y, width, height), *paint.sk_paint());
}

void Canvas::DrawRect(const RectF &rect, const Paint &paint) const {
  sk_canvas_->drawRect(SkRect::MakeLTRB(rect.left, rect.top, rect.right, rect.bottom), *paint.sk_paint());
}

void Canvas::DrawRoundRect(const Rect &rect, float rx, float ry, const Paint &paint) const {
  sk_canvas_->drawRoundRect(SkRect::MakeLTRB(rect.left, rect.top, rect.right, rect.bottom), rx, ry, *paint.sk_paint());
}

void Canvas::DrawRoundRect(const RectF &rect, float rx, float ry, const Paint &paint) const {
  sk_canvas_->drawRoundRect(SkRect::MakeLTRB(rect.left, rect.top, rect.right, rect.bottom), rx, ry, *paint.sk_paint());
}

void Canvas::DrawCircle(float x, float y, float radius, const Paint &paint) const {
  sk_canvas_->drawCircle(x, y, radius, *paint.sk_paint());
}

void Canvas::DrawPath(const Path &path, const Paint &paint) const {
  sk_canvas_->drawPath(*path.sk_path(), *paint.sk_paint());
}

void Canvas::Translate(float dx, float dy) const {
  sk_canvas_->translate(dx, dy);
}

void Canvas::ResetMatrix() const {
  sk_canvas_->resetMatrix();
}

void Canvas::Clear(uint32_t color) const {
  sk_canvas_->clear(color);
}

void Canvas::Clear(const Color &color) const {
  sk_canvas_->clear(color.argb());
}

void Canvas::SetMargin(const Margin &margin) {
  margin_ = margin;
  sk_canvas_->resetMatrix();
  sk_canvas_->translate(margin_.left, margin_.top);
}

}
