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

namespace skland {

// Forward declaration
class Application;
class Canvas;

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

  void Reset();

 private:

  Theme();

  ~Theme();

  ColorScheme window_frame_;  // The default window frame

  static Theme *kTheme;

};

}

#endif // SKLAND_STOCK_THEME_HPP_
