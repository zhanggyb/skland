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
#include <skland/graphic/font.hpp>
#include <skland/graphic/shader.hpp>

#include "SkPaint.h"
#include "SkTypeface.h"

#include "internal/font-meta.hpp"
#include "internal/shader-meta.hpp"

namespace skland {

Paint::Paint() {
  metadata_.reset(new SkPaint);
}

Paint::Paint(const Paint &orig) {
  metadata_.reset(new SkPaint(*orig.metadata_));
}

Paint::~Paint() {
}

Paint &Paint::operator=(const Paint &other) {
  *metadata_ = *other.metadata_;
  return *this;
}

uint32_t Paint::GetHash() const {
  return metadata_->getHash();
}

void Paint::Reset() {
  metadata_->reset();
}

Paint::Hinting Paint::GetHinting() const {
  return static_cast<Hinting>(metadata_->getHinting());
}

void Paint::SetHinting(Hinting hinting) {
  metadata_->setHinting(static_cast<SkPaint::Hinting >(hinting));
}

bool Paint::IsAntiAlias() const {
  return metadata_->isAntiAlias();
}

void Paint::SetAntiAlias(bool aa) {
  metadata_->setAntiAlias(aa);
}

void Paint::SetStyle(Style style) {
  metadata_->setStyle(static_cast<SkPaint::Style>(style));
}

Color Paint::GetColor() const {
  uint32_t value = metadata_->getColor();
  return Color::FromUCharRGBA(SkToU8(SkColorGetR(value)),
                              SkToU8(SkColorGetG(value)),
                              SkToU8(SkColorGetB(value)),
                              SkToU8(SkColorGetA(value)));
}

void Paint::SetColor(uint32_t argb) {
  metadata_->setColor(argb);
}

void Paint::SetColor(const Color &color) {
  metadata_->setARGB(color.uchar_alpha(),
                     color.uchar_red(),
                     color.uchar_green(),
                     color.uchar_blue());
}

float Paint::GetStrokeWidth() const {
  return metadata_->getStrokeWidth();
}

void Paint::SetStrokeWidth(float width) {
  metadata_->setStrokeWidth(width);
}

void Paint::SetFont(const Font &font) {
  SkTypeface *ptr = font.GetSkTypeface();
  sk_sp<SkTypeface> typeface = SkTypeface::MakeFromTypeface(ptr, ptr->style());
  metadata_->setTypeface(typeface);
}

void Paint::SetShader(const Shader &shader) {
  metadata_->setShader(shader.metadata_->sk_shader);
}

Paint::Align Paint::GetTextAlign() const {
  return static_cast<Align>(metadata_->getTextAlign());
}

void Paint::SetTextAlign(Align align) {
  metadata_->setTextAlign(static_cast<SkPaint::Align>(align));
}

float Paint::GetTextSize() const {
  return metadata_->getTextSize();
}

void Paint::SetTextSize(float size) {
  metadata_->setTextSize(size);
}

float Paint::GetTextScaleX() const {
  return metadata_->getTextScaleX();
}

void Paint::SetTextScaleX(float scale_x) {
  metadata_->setTextScaleX(scale_x);
}

float Paint::GetTextSkewX() const {
  return metadata_->getTextSkewX();
}

void Paint::SetTextSkewX(float skew_x) {
  metadata_->setTextSkewX(skew_x);
}

TextEncoding Paint::GetTextEncoding() const {
  return static_cast<TextEncoding>(metadata_->getTextEncoding());
}

void Paint::SetTextEncoding(TextEncoding encoding) {
  metadata_->setTextEncoding(static_cast<SkPaint::TextEncoding>(encoding));
}

int Paint::TextToGlyphs(const void *text, size_t byte_length, uint16_t *glyphs) const {
  return metadata_->textToGlyphs(text, byte_length, glyphs);
}

bool Paint::ContainsText(const void *text, size_t byte_length) const {
  return metadata_->containsText(text, byte_length);
}

void Paint::GlyphsToUnichars(const uint16_t glyphs[], int count, int32_t text[]) const {
  metadata_->glyphsToUnichars(glyphs, count, text);
}

int Paint::CountText(const void *text, size_t byte_length) const {
  return metadata_->countText(text, byte_length);
}

float Paint::MeasureText(const void *text, size_t length, Rect *boulds) const {
  return metadata_->measureText(text, length, reinterpret_cast<SkRect *>(boulds));
}

bool operator==(const Paint &paint1, const Paint &paint2) {
  return paint1.metadata_ == paint2.metadata_;
}

bool operator!=(const Paint &paint1, const Paint &paint2) {
  return paint1.metadata_ != paint2.metadata_;
}

}
