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

#include "../core/types.hpp"
#include "../core/rect.hpp"

#include "font-style.hpp"

#include <memory>

class SkTypeface;

namespace skland {

typedef uint32_t FontID;
typedef uint32_t FontTableTag;

class Font;

/**
 * @ingroup graphic
 * @brief Font typeface
 */
class Typeface {

  friend class Font;
  friend bool operator==(const Typeface &typeface1, const Typeface &typeface2);

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

  Typeface(Style style = Style::kNormal);

  Typeface(const char *family_name, FontStyle font_style);

  Typeface(const Typeface &other, Style style);

  Typeface(SkTypeface *family, Style style);

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

  FontID GetUniqueID() const;

  int CharsToGlyphs(const void *chars, Encoding encoding, GlyphID glyphs[], int glyph_count) const;

  int CountGlyphs() const;

  int CountTables() const;

  int GetTableTags(FontTableTag tags[]) const;

  size_t GetTableSize(FontTableTag tag) const;

  size_t GetTableData(FontTableTag tag, size_t offset, size_t lengh, void *data) const;

  int GetUnitsPerEm() const;

  bool GetKerningPairAdjustments(const GlyphID glyphs[], int count, int32_t adjustments[]) const;

  Rect GetBounds() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

bool operator==(const Typeface &face1, const Typeface &face2);

}

#endif // SKLAND_GRAPHIC_TYPEFACE_HPP_
