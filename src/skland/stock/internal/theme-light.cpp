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

#include "theme-light.hpp"

#include <skland/stock/theme.hpp>

namespace skland {

/**
 * @brief The default light theme
 */
class ThemeLight : public Theme {

 public:

  ThemeLight()
      : Theme() {
    window_schema().background = 0xEFF0F0F0;
    window_schema().background_active = 0xEFF0F0F0;
    window_schema().foreground = 0xEFE0E0E0;
    window_schema().foreground_active = 0xEFE0E0E0;

    title_bar_schema().background = 0xEFF0F0F0;
    title_bar_schema().background_active = 0xEFE0E0E0;
    title_bar_schema().background_highlight = title_bar_schema().background_active + 55;
    title_bar_schema().foreground = 0xFF444444;
    title_bar_schema().foreground_active = 0xFF999999;
    title_bar_schema().foreground_highlight = title_bar_schema().foreground_active + 55;
  }

  ~ThemeLight() {}

};

}

void *ThemeLightCreate() {
  return new skland::ThemeLight;
}

void ThemeLightDestroy(void *p) {
  skland::ThemeLight *theme = static_cast<skland::ThemeLight *>(p);
  delete theme;
}

