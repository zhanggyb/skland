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

#ifndef SKLAND_GRAPHIC_INTERNAL_FONT_META_HPP_
#define SKLAND_GRAPHIC_INTERNAL_FONT_META_HPP_

#include <skland/graphic/font.hpp>

#include "SkTypeface.h"

namespace skland {

struct FontMeta {

  FontMeta()
      : sk_font_style() {
  }

  FontMeta(const char *name, int weight, int width, SkFontStyle::Slant slant)
      : sk_font_style(weight, width, slant) {
    sk_typeface = SkTypeface::MakeFromName(name, sk_font_style);
  }

  FontMeta(const FontMeta &other)
      : sk_font_style(other.sk_font_style), sk_typeface(other.sk_typeface) {
  }

  FontMeta &operator=(const FontMeta &other) {
    sk_typeface = other.sk_typeface;
    sk_font_style = other.sk_font_style;
    return *this;
  }

  SkFontStyle sk_font_style;
  sk_sp<SkTypeface> sk_typeface;

};

}

#endif // SKLAND_GRAPHIC_INTERNAL_FONT_META_HPP_
