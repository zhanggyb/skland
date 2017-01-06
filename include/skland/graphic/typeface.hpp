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

#ifndef SKLAND_GRAPHIC_TYPEFACE_HPP_
#define SKLAND_GRAPHIC_TYPEFACE_HPP_

#include "../core/rect.hpp"
#include "font-style.hpp"

#include <memory>

namespace skland {

class Font;
struct TypefaceMeta;

class Typeface {

  friend class Font;

 public:

  enum Style {
    kNormal = 0,
    kBold = 0x01,
    kItalic = 0x02,

    kBoldItalic = 0x03
  };

  enum Encoding {
    kEncodingUTF8,
    kEncodingUTF16,
    kEncodingUTF32
  };

  Typeface(Style style = kNormal);

  Typeface(const char *family_name, FontStyle font_style);

  Typeface(const Typeface &other, Style style);

  /**
   * Create a new typeface by given file.
   * @param path
   * @param index
   */
  Typeface(const char *path, int index = 0);

  ~Typeface();

  Typeface &operator=(const Typeface &other);

  FontStyle GetFontStyle() const;

  Style GetStyle() const;

  bool IsBold() const;

  bool IsItalic() const;

  bool IsFixedPitch() const;

  uint32_t GetUniqueID() const;

  int CharsToGlyphs(const void *chars, Encoding encoding, uint16_t glyphs[], int glyph_count) const;

  int CountGlyphs() const;

  int CountTables() const;

  int GetTableTags(uint32_t tags[]) const;

  size_t GetTableSize(uint32_t tag) const;

  size_t GetTableData(uint32_t tag, size_t offset, size_t lengh, void *data) const;

  int GetUnitsPerEm() const;

  bool GetKerningPairAdjustments(const uint16_t glyphs[], int count, int32_t adjustments[]) const;

  Rect GetBounds() const;

 private:

  std::unique_ptr<TypefaceMeta> metadata_;

};

bool operator==(const Typeface &face1, const Typeface &face2);

}

#endif // SKLAND_GRAPHIC_TYPEFACE_HPP_
