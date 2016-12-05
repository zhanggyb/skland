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

#include <skland/graphic/paint.hpp>

#include "SkPaint.h"

namespace skland {

Paint::Paint()
    : sk_paint_(nullptr) {
  sk_paint_ = new SkPaint;
}

Paint::Paint(const Paint &orig)
    : sk_paint_(nullptr) {
  sk_paint_ = new SkPaint(*orig.sk_paint_);
}

Paint::~Paint() {
  delete sk_paint_;
}

Paint &Paint::operator=(const Paint &other) {
  *sk_paint_ = *other.sk_paint_;
  return *this;
}

uint32_t Paint::GetHash() const {
  return sk_paint_->getHash();
}

void Paint::Reset() {
  sk_paint_->reset();
}

Paint::Hinting Paint::GetHinting() const {
  return static_cast<Hinting>(sk_paint_->getHinting());
}

void Paint::SetHinting(Hinting hinting) {
  sk_paint_->setHinting(static_cast<SkPaint::Hinting >(hinting));
}

bool Paint::IsAntiAlias() const {
  return sk_paint_->isAntiAlias();
}

void Paint::SetAntiAlias(bool aa) {
  sk_paint_->setAntiAlias(aa);
}

void Paint::SetStyle(Style style) {
  sk_paint_->setStyle(static_cast<SkPaint::Style>(style));
}

Color Paint::GetColor() const {
  uint32_t value = sk_paint_->getColor();
  return Color::FromUCharRGBA(SkToU8(SkColorGetR(value)),
                              SkToU8(SkColorGetG(value)),
                              SkToU8(SkColorGetB(value)),
                              SkToU8(SkColorGetA(value)));
}

void Paint::SetColor(uint32_t argb) {
  sk_paint_->setColor(argb);
}

void Paint::SetColor(const Color &color) {
  sk_paint_->setARGB(color.uchar_alpha(),
                     color.uchar_red(),
                     color.uchar_green(),
                     color.uchar_blue());
}

float Paint::GetStrokeWidth() const {
  return sk_paint_->getStrokeWidth();
}

void Paint::SetStrokeWidth(float width) {
  sk_paint_->setStrokeWidth(width);
}

}
