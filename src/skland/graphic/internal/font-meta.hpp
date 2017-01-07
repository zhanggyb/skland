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

#include "SkFont.h"

namespace skland {

struct FontMeta {

  FontMeta() {}

  FontMeta(const sk_sp<SkFont> &font)
      : sk_font(font) {}

  FontMeta(const FontMeta &other)
      : sk_font(other.sk_font) {}

  FontMeta &operator=(const FontMeta &other) {
    sk_font = other.sk_font;
    return *this;
  }

  FontMeta &operator=(const sk_sp<SkFont> &font) {
    sk_font = font;
    return *this;
  }

  sk_sp<SkFont> sk_font;

};

}

#endif // SKLAND_GRAPHIC_INTERNAL_FONT_META_HPP_
