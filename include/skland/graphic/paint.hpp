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

#ifndef SKLAND_GRAPHIC_PAINT_HPP_
#define SKLAND_GRAPHIC_PAINT_HPP_

#include "../core/color.hpp"
#include "font.hpp"

#include <cstdint>
#include <memory>

class SkPaint;

namespace skland {

class Shader;

class Paint {

  friend bool operator==(const Paint &paint1, const Paint &paint2);
  friend bool operator!=(const Paint &paint1, const Paint &paint2);

 public:

  enum Style {
    kStyleFill,   //!< fill the geometry
    kStyleStroke,      //!< stroke the geometry
    kStyleStrokeAndFill    //!< fill and stroke the geometry
  };

  enum Hinting {
    kHintingNone,
    kHintingSlight,
    kHintingNormal,
    kHintingFull
  };

  enum Align {
    kAlignLeft,
    kAlignCenter,
    kAlignRight
  };

  enum {
    kAlignCount = 3
  };

  Paint();

  Paint(const Paint &orig);

  ~Paint();

  Paint &operator=(const Paint &other);

  uint32_t GetHash() const;

  void Reset();

  Hinting GetHinting() const;

  void SetHinting(Hinting hinting);

  bool IsAntiAlias() const;

  void SetAntiAlias(bool aa);

  void SetStyle(Style style);

  Color GetColor() const;

  void SetColor(uint32_t argb);

  void SetColor(const Color &color);

  float GetStrokeWidth() const;

  void SetStrokeWidth(float width);

  void SetFont(const Font &font);

  void SetShader(const Shader &shader);

  Align GetTextAlign() const;

  void SetTextAlign(Align align);

  float GetTextSize() const;

  void SetTextSize(float size);

  float GetTextScaleX() const;

  void SetTextScaleX(float scale_x);

  float GetTextSkewX() const;

  void SetTextSkewX(float skew_x);

  TextEncoding GetTextEncoding() const;

  void SetTextEncoding(TextEncoding encoding);

  int TextToGlyphs(const void *text, size_t byte_length, uint16_t glyphs[]) const;

  bool ContainsText(const void *text, size_t byte_length) const;

  void GlyphsToUnichars(const uint16_t glyphs[], int count, int32_t text[]) const;

  int CountText(const void *text, size_t byte_length) const;

  float MeasureText(const void *text, size_t length, Rect *boulds) const;

  float MeasureText(const void *text, size_t length) const {
    return MeasureText(text, length, nullptr);
  }

  SkPaint *sk_paint() const {
    return metadata_.get();
  }

 private:

  std::unique_ptr<SkPaint> metadata_;
};

bool operator==(const Paint &paint1, const Paint &paint2);

bool operator!=(const Paint &paint1, const Paint &paint2);

}

#endif // SKLAND_GRAPHIC_PAINT_HPP_
