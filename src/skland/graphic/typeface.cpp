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

#include "internal/typeface-private.hpp"

namespace skland {

Typeface::Typeface(Style style) {
  p_.reset(new Private);
  p_->sk_typeface = SkTypeface::MakeDefault(static_cast<SkTypeface::Style>(style));
}

Typeface::Typeface(const char *family_name, FontStyle font_style) {
  p_.reset(new Private);
  p_->sk_typeface = SkTypeface::MakeFromName(family_name, *reinterpret_cast<SkFontStyle *>(&font_style));
}

Typeface::Typeface(const Typeface &other, Style style) {
  p_.reset(new Private);
  p_->sk_typeface = SkTypeface::MakeFromTypeface(other.p_->sk_typeface.get(), (SkTypeface::Style) style);
}

Typeface::Typeface(const char *path, int index) {
  p_.reset(new Private);
  p_->sk_typeface = SkTypeface::MakeFromFile(path, index);
}

Typeface::~Typeface() {

}

Typeface &Typeface::operator=(const Typeface &other) {
  p_->sk_typeface = other.p_->sk_typeface;
  return *this;
}

FontStyle Typeface::GetFontStyle() const {
  return FontStyle(p_->sk_typeface->fontStyle());
}

Typeface::Style Typeface::GetStyle() const {
  return (Style) p_->sk_typeface->style();
}

bool Typeface::IsBold() const {
  return p_->sk_typeface->isBold();
}

bool Typeface::IsItalic() const {
  return p_->sk_typeface->isItalic();
}

bool Typeface::IsFixedPitch() const {
  return p_->sk_typeface->isFixedPitch();
}

uint32_t Typeface::GetUniqueID() const {
  return p_->sk_typeface->uniqueID();
}

bool operator==(const Typeface &typeface1, const Typeface &typeface2) {
  return SkTypeface::Equal(typeface1.p_->sk_typeface.get(), typeface2.p_->sk_typeface.get());
}

}
