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

    struct ShadedColor {

      ShadedColor()
          : shaded(false), shaded_count(0) {}

      ~ShadedColor() {}

      Color color;
      bool shaded;
      int shaded_count;
      std::vector<uint32_t> shaded_colors;
      std::vector<float> shaded_positions;

    };

    Schema() {
      background.color = 0xEFF0F0F0;
      background_active.color = 0xEFF0F0F0;
      background_highlight.color = background_active.color + 25;
      foreground.color = 0xFF444444;
      foreground_active.color = 0xFF999999;
      foreground_highlight.color = foreground_active.color + 25;
    }

    ~Schema() {}

    // TODO: use image
    ShadedColor outline;

    ShadedColor outline_active;

    ShadedColor outline_highlight;

    ShadedColor background;

    ShadedColor background_active;

    ShadedColor background_highlight;

    ShadedColor foreground;

    ShadedColor foreground_active;

    ShadedColor foreground_highlight;

  };

  struct Data {

    std::string name;

    Schema window;

    Schema title_bar;

    Schema button;

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

}

#endif // SKLAND_STOCK_THEME_HPP_
