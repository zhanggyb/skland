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

#ifndef SKLAND_GRAPHIC_FONT_STYLE_HPP_
#define SKLAND_GRAPHIC_FONT_STYLE_HPP_

#include "../core/defines.hpp"

#include <memory>

class SkFontStyle;

namespace skland {

/**
 * @ingroup graphic
 * @brief Font style
 *
 * This class has the same structure of SkFontStyle so it can be reinterpreted cast to each other.
 */
SKLAND_EXPORT class FontStyle {

 public:

  enum Weight {
    kWeightInvisible = 0,
    kWeightThin = 100,
    kWeightExtraLight = 200,
    kWeightLight = 300,
    kWeightNormal = 400,
    kWeightMedium = 500,
    kWeightSemiBold = 600,
    kWeightBold = 700,
    kWeightExtraBold = 800,
    kWeightBlack = 900,
    kWeightExtraBlack = 1000,
  };

  enum Width {
    kWidthUltraCondensed = 1,
    kWidthExtraCondensed = 2,
    kWidthCondensed = 3,
    kWidthSemiCondensed = 4,
    kWidthNormal = 5,
    kWidthSemiExpanded = 6,
    kWidthExpanded = 7,
    kWidthExtraExpanded = 8,
    kWidthUltraExpanded = 9,
  };

  enum Slant {
    kSlantUpright,
    kSlantItalic,
    kSlantOblique
  };

  FontStyle();

  FontStyle(int weight, int width, Slant slant);

  FontStyle(const FontStyle &other);

  FontStyle(const SkFontStyle &sk_font_style);

  FontStyle &operator=(const FontStyle &other);

  bool operator==(const FontStyle &other) const {
    return data_.value == other.data_.value;
  }

  int weight() const { return data_.styles.weight; }

  int width() const { return data_.styles.width; }

  Slant slant() const { return (Slant) data_.styles.slant; }

 private:

  union {
    struct {
      uint16_t weight;
      uint8_t width;
      uint8_t slant;
    } styles;
    uint32_t value;
  } data_;

};

}

#endif //SKLAND_FONT_STYLE_HPP
