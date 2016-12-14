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

#include <skland/graphic/font.hpp>

#include "internal/meta-font.hpp"

namespace skland {

Font::Font(const char *name, Weight weight, Width width, Slant slant)
    : metadata_(nullptr) {
  metadata_ = new graphic::MetaFont(name, (int) weight, (int) width, (SkFontStyle::Slant) slant);
}

Font::Font(const Font &other)
    : metadata_(nullptr) {
  metadata_ = new graphic::MetaFont(*other.metadata_);
}

Font::~Font() {
  delete metadata_;
}

Font &Font::operator=(const Font &other) {
  *metadata_ = *other.metadata_;
  return *this;
}

void Font::LoadName(const char *name, Weight weight, Width width, Slant slant) {
  metadata_->sk_font_style = SkFontStyle((int) weight, (int) width, (SkFontStyle::Slant) slant);
  metadata_->sk_typeface = SkTypeface::MakeFromName(name, metadata_->sk_font_style);
}

void Font::SetBold(bool bold) {
  if (bold != metadata_->sk_typeface->isBold()) {
    metadata_->sk_typeface = SkTypeface::MakeFromTypeface(metadata_->sk_typeface.get(), SkTypeface::kBold);
  }
}

bool Font::IsBold() const {
  return metadata_->sk_typeface->isBold();
}

void Font::SetItalic(bool italic) {
  if (italic != metadata_->sk_typeface->isItalic()) {
    metadata_->sk_typeface = SkTypeface::MakeFromTypeface(metadata_->sk_typeface.get(), SkTypeface::kItalic);
  }
}

bool Font::IsItalic() const {
  return metadata_->sk_typeface->isItalic();
}

}
