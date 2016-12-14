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

#include <cstdint>
#include <skland/core/color.hpp>

class SkPaint;

namespace skland {

class Font;

class Paint {

 public:

  enum Style {
    kStyleFill, // kFill_Style,            //!< fill the geometry
    kStyleStroke, // kStroke_Style,          //!< stroke the geometry
    kStyleStrokeAndFill // kStrokeAndFill_Style,   //!< fill and stroke the geometry
  };

  enum Hinting {
    kHintingNone, // kNo_Hinting            = 0,
    kHintingSlight, // kSlight_Hinting        = 1,
    kHintingNormal, // kNormal_Hinting        = 2,     //!< this is the default
    kHintingFull  // kFull_Hinting          = 3
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

  void SetTextSize(float size);

  SkPaint *sk_paint() const {
    return sk_paint_;
  }

 private:

  SkPaint *sk_paint_;
};

}

#endif // SKLAND_GRAPHIC_PAINT_HPP_
