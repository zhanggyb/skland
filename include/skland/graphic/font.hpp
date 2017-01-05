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

#ifndef SKLAND_GRAPHIC_FONT_HPP_
#define SKLAND_GRAPHIC_FONT_HPP_

#include "font-style.hpp"
#include <memory>

namespace skland {

// Foreward declaration
class Paint;
struct FontMeta;
class Typeface;

class Font {

  friend class Paint;

 public:

  enum Flags {
    kFlagEnableAutoHints = 1 << 0,
    kFlagEnableByteCodeHints = 1 << 1,
    kFlagEmbeddedBitmaps = 1 << 2,
    kFlagUseNonlinearMetrics = 1 << 3,

    kFlagVertical = 1 << 4,
    kFlagGenA8FromLCD = 1 << 5,
    kFlagEmbolden = 1 << 6,
    kFlagDevKern = 1 << 7
  };

  enum MaskType {
    kMaskBW,
    kMaskA8,
    kMaskLCD
  };

  Font(const char *name = "Arial", FontStyle font_style = FontStyle());

  Font(const Typeface &typeface, float size, MaskType mask_type, uint32_t flags);

  Font(const Font &other);

  ~Font();

  Font &operator=(const Font &other);

 private:

  std::unique_ptr<FontMeta> metadata_;

};

}

#endif // SKLAND_GRAPHIC_FONT_HPP_
