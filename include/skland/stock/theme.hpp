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

#include "../graphic/shader.hpp"

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

  struct Schema {

    Schema()
        : outline(0x0),
          outline_active(0x0),
          outline_highlight(0x0),
          background(0xEFF0F0F0),
          background_active(0xEFE0E0E0),
          background_highlight(background_active + 25),
          foreground(0xFF444444),
          foreground_active(0xFF999999),
          foreground_highlight(foreground_active + 25) {}

    ~Schema() {}

    Color outline;
    Shader outline_shader;

    Color outline_active;
    Shader outline_active_shader;

    Color outline_highlight;
    Shader outline_highlight_shader;

    Color background;
    Shader background_shader;

    Color background_active;
    Shader background_active_shader;

    Color background_highlight;
    Shader background_highlight_shader;

    Color foreground;
    Shader foreground_shader;

    Color foreground_active;
    Shader foreground_active_shader;

    Color foreground_highlight;
    Shader foreground_highlight_shader;
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

  static const Schema &GetWindowSchema() {
    return kTheme->window_schema_;
  }

  static const Schema &GetTitleBarSchema() {
    return kTheme->title_bar_schema_;
  }

 protected:

  Theme();

  virtual ~Theme();

  Schema &window_schema() {
    return window_schema_;
  };

  Schema &title_bar_schema() {
    return title_bar_schema_;
  }

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

  Schema window_schema_;

  Schema title_bar_schema_;

  static Theme *kTheme;

};

}

#endif // SKLAND_STOCK_THEME_HPP_
