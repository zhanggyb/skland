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
#include <skland/graphic/typeface.hpp>

#include "internal/font-meta.hpp"
#include "internal/typeface-meta.hpp"

namespace skland {

Font::Font(float size, MaskType mask_type, uint32_t flags) {
  metadata_.reset(new FontMeta);
  sk_sp<SkTypeface> typeface = SkTypeface::MakeDefault();
  metadata_->sk_font = SkFont::Make(typeface, size, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const char *family_name, FontStyle font_style, float size, MaskType mask_type, uint32_t flags) {
  metadata_.reset(new FontMeta);
  sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName(family_name, *reinterpret_cast<SkFontStyle *>(&font_style));
  metadata_->sk_font = SkFont::Make(typeface, size, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const Typeface &typeface, float size, MaskType mask_type, uint32_t flags) {
  metadata_.reset(new FontMeta);
  metadata_->sk_font = SkFont::Make(typeface.metadata_->sk_typeface, size, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const Typeface &typeface, float size, float scale_x, float skew_x, MaskType mask_type, uint32_t flags) {
  metadata_.reset(new FontMeta);
  metadata_->sk_font =
      SkFont::Make(typeface.metadata_->sk_typeface, size, scale_x, skew_x, (SkFont::MaskType) mask_type, flags);
}

Font::Font(const Font &other) {
  metadata_.reset(new FontMeta);
  metadata_->sk_font = other.metadata_->sk_font;
}

Font::~Font() {
}

Font &Font::operator=(const Font &other) {
  metadata_->sk_font = other.metadata_->sk_font;
  return *this;
}

}
