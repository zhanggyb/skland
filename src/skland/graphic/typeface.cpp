//
// Created by zhanggyb on 17-1-5.
//

#include <skland/graphic/typeface.hpp>

#include "internal/typeface-meta.hpp"

namespace skland {

Typeface::Typeface(Style style) {
  metadata_.reset(new TypefaceMeta);
  metadata_->sk_typeface = SkTypeface::MakeDefault((SkTypeface::Style) style);
}

Typeface::Typeface(const char *family_name, FontStyle font_style) {
  metadata_.reset(new TypefaceMeta);
  metadata_->sk_typeface = SkTypeface::MakeFromName(family_name, *reinterpret_cast<SkFontStyle *>(&font_style));
}

Typeface::Typeface(const Typeface &other, Style style) {
  metadata_.reset(new TypefaceMeta);
  metadata_->sk_typeface = SkTypeface::MakeFromTypeface(other.metadata_->sk_typeface.get(), (SkTypeface::Style) style);
}

Typeface::Typeface(const char *path, int index) {
  metadata_.reset(new TypefaceMeta);
  metadata_->sk_typeface = SkTypeface::MakeFromFile(path, index);
}

Typeface::~Typeface() {

}

Typeface &Typeface::operator=(const Typeface &other) {
  metadata_->sk_typeface = other.metadata_->sk_typeface;
  return *this;
}

}
