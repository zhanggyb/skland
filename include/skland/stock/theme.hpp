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

#include "../core/color.hpp"
#include "../core/margin.hpp"

#include <vector>
#include <string>

class SkPixmap;

namespace skland {

typedef void *(*ThemeCreateHandle)();
typedef void(*ThemeDestroyHandle)(void *p);

/**
 * @ingroup stock
 * @brief The global theme manager
 */
class Theme {

  friend class Application;

  Theme(const Theme &) = delete;
  Theme &operator=(const Theme &) = delete;

 public:

  struct ColorScheme {

    ColorScheme()
        : outline(),
          item(),
          inner(0xEFF0F0F0),
          inner_selected(0xEFE0E0E0),
          text(0xFF444444),
          text_selected(0xFF999999),
          shaded(false),
          shadetop(0),
          shadedown(0),
          alpha_check(false) {}

    Color outline;
    Color item;
    Color inner;
    Color inner_selected;
    Color text;
    Color text_selected;
    bool shaded;
    short shadetop;
    short shadedown;
    bool alpha_check;
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

  static const ColorScheme &GetWindowColorScheme() {
    return kTheme->window_color_scheme_;
  }

 protected:

  Theme();

  virtual ~Theme();

  ColorScheme &window_color_scheme() {
    return window_color_scheme_;
  };

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

  std::string name_;

  ColorScheme window_color_scheme_;

  static Theme *kTheme;

};

}

#endif // SKLAND_STOCK_THEME_HPP_
