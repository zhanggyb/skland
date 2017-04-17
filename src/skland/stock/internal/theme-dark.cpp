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
#include <skland/graphic/gradient-shader.hpp>

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
    window_schema().foreground_active = 0xEF404040;

    title_bar_schema().background = 0xEF202020;
    Point2F points[2];
    points[0].x = 0.f;
    points[0].y = 0.f;
    points[1].x = 0.f;
    points[1].y = 22.f;
    uint32_t colors[2] = {0xFF303030, 0xFF292929};
    float pos[2] = {0.f, 1.f};
    title_bar_schema().background_active_shader =
        GradientShader::MakeLinear(points, colors, pos, 2, Shader::TileMode::kTileModeClamp);
    title_bar_schema().background_highlight = title_bar_schema().background_active + 55;
    title_bar_schema().foreground = 0xFF999999;
    title_bar_schema().foreground_active = 0xFFA0A0A0;
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
