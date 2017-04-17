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

#include "theme-dark.hpp"

#include <skland/stock/theme.hpp>

namespace skland {

/**
 * @brief The built-in dark theme
 */
class ThemeDark : public Theme {

 public:

  ThemeDark()
      : Theme() {
    window_schema().background = 0xEF202020;
    window_schema().background_active = 0xEF202020;
    window_schema().foreground = 0xEF303030;
    window_schema().foreground_active = 0xEF303030;

    title_bar_schema().background = 0xEF202020;
    title_bar_schema().background_active = 0xEF303030;
    title_bar_schema().background_highlight = title_bar_schema().background_active + 55;
    title_bar_schema().foreground = 0xFF999999;
    title_bar_schema().foreground_active = 0xFF444444;
    title_bar_schema().foreground_highlight = title_bar_schema().foreground_active + 55;
  }

  ~ThemeDark() {}

};

}

void *ThemeDarkCreate() {
  return new skland::ThemeDark;
}

void ThemeDarkDestroy(void *p) {
  skland::ThemeDark *theme = static_cast<skland::ThemeDark *>(p);
  delete theme;
}
