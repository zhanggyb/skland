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

#include <skland/core/color.hpp>

#include <vector>

class SkPixmap;

namespace skland {

// Forward declaration
class Application;

struct ColorScheme {
  ColorD outline;
  ColorD item;
  ColorD inner;
  ColorD inner_selected;
  ColorD text;
  ColorD text_selected;
  bool shaded;
  short shadetop;
  short shadedown;
  bool alpha_check;
};

class Theme {

  friend class Application;

  Theme(const Theme &orig) = delete;
  Theme &operator=(const Theme &other) = delete;

 public:

  static inline const ColorScheme &window_frame() {
    return kTheme->window_frame_;
  }

  static inline int shadow_radius() {
    return kTheme->shadow_radius_;
  }

  static inline int shadow_offset_x() {
    return kTheme->shadow_offset_x_;
  }

  static inline int shadow_offset_y() {
    return kTheme->shadow_offset_y_;
  }

  static inline int shadow_margin_left() {
    return kTheme->shadow_radius_ - kTheme->shadow_offset_x_;
  }

  static inline int shadow_margin_right() {
    return kTheme->shadow_radius_ + kTheme->shadow_offset_x_;
  }

  static inline int shadow_margin_top() {
    return kTheme->shadow_radius_ - kTheme->shadow_offset_y_;
  }

  static inline int shadow_margin_bottom() {
    return kTheme->shadow_radius_ + kTheme->shadow_offset_y_;
  }

  static inline const SkPixmap *shadow_pixmap() {
    return kTheme->shadow_pixmap_;
  }

  void Reset();

 private:

  Theme();

  ~Theme();

  void GenerateShadowImage();

  ColorScheme window_frame_;  // The default window frame

  int shadow_radius_;

  int shadow_offset_x_;
  int shadow_offset_y_;

  std::vector<uint32_t> shadow_pixels_;

  SkPixmap *shadow_pixmap_;

  static const int kShadowPixelsWidth = 250;

  static const int kShadowPixelsHeight = 250;

  static Theme *kTheme;

};

}

#endif // SKLAND_STOCK_THEME_HPP_
