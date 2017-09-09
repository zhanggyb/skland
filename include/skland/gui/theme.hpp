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

#ifndef SKLAND_STOCK_THEME_HPP_
#define SKLAND_STOCK_THEME_HPP_

#include "skland/core/color.hpp"
#include "skland/core/margin.hpp"
#include "skland/core/point.hpp"

#include "skland/graphic/shader.hpp"
#include "skland/graphic/font.hpp"

#include <vector>
#include <string>

class SkPixmap;

namespace skland {
namespace gui {

typedef void *(*ThemeCreateHandle)();
typedef void(*ThemeDestroyHandle)(void *p);

/**
 * @ingroup stock
 * @brief The global theme manager
 */
class Theme {

  friend class Application;

 public:

  Theme(const Theme &) = delete;
  Theme &operator=(const Theme &) = delete;

  using Margin = core::Margin;

  struct Schema {

    struct ShadedColor {

      ShadedColor() = default;
      ~ShadedColor() = default;
      ShadedColor(const ShadedColor &) = default;
      ShadedColor &operator=(const ShadedColor &) = default;

      core::ColorF color;
      bool shaded = false;
      int shaded_count = 0;
      std::vector<uint32_t> shaded_colors;
      std::vector<float> shaded_positions;

    };

    struct ShadedColorGroup {

      ShadedColorGroup() = default;
      ~ShadedColorGroup() = default;
      ShadedColorGroup(const ShadedColorGroup &) = default;
      ShadedColorGroup &operator=(const ShadedColorGroup &) = default;
      ShadedColor foreground;
      ShadedColor background;
      ShadedColor outline;

    };

    Schema() {

      active.foreground.color = 0xFF000000; // black
      active.background.color = 0xFFFFFFFF; // white
      active.outline.color = 0xFF000000;  // black

      inactive = active;
      highlight = highlight;

    }

    ~Schema() = default;

    // TODO: use image

    ShadedColorGroup active;
    ShadedColorGroup inactive;
    ShadedColorGroup highlight;

  };

  struct Data {

    Data();

    std::string name;

    Schema window;

    Schema title_bar;

    graphic::Font title_bar_font;

    Schema button;

    graphic::Font default_font;

  };

  struct Helper {

    struct GradientShader {

      static graphic::Shader MakeLinear(const core::PointF points[2], const Schema::ShadedColor &color);

    };

  };

  static void Load(const char *name = nullptr);

  static inline int GetShadowRadius() {
    return kShadowRadius;
  }

  static inline int GetShadowOffsetX() {
    return kShadowOffsetX;
  }

  static inline int GetShadowOffsetY() {
    return kShadowOffsetY;
  }

  static inline const Margin &GetShadowMargin() {
    return kShadowMargin;
  }

  static inline const SkPixmap *GetShadowPixmap() {
    return kShadowPixmap;
  }

  static const int kShadowImageWidth = 250;

  static const int kShadowImageHeight = 250;

  static const Data &GetData() { return kTheme->data_; }

 protected:

  Theme();

  virtual ~Theme();

  Data &data() { return data_; }

 private:

  /**
   * @brief Initialize static properties
   *
   * This method is called only in Application
   */
  static void Initialize();

  /**
   * @brief Release the memory allocated for theme
   *
   * This method is called only in Application
   */
  static void Release();

  static void GenerateShadowImage();

  static int kShadowRadius;

  static int kShadowOffsetX;
  static int kShadowOffsetY;

  static Margin kShadowMargin;

  static std::vector<uint32_t> kShadowPixels;

  static SkPixmap *kShadowPixmap;

  static Theme *kTheme;

  Data data_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_STOCK_THEME_HPP_
