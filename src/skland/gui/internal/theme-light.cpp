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

#include <skland/gui/theme.hpp>

namespace skland {
namespace gui {

/**
 * @brief The default light theme
 */
class ThemeLight : public Theme {

 public:

  ThemeLight()
      : Theme() {
    data().window.inactive.outline.color = 0xFF000000;
    data().window.inactive.background.color = 0xEFF0F0F0;
    data().window.active.background.color = 0xEFF0F0F0;
    data().window.inactive.foreground.color = 0xEFE0E0E0;
    data().window.active.foreground.color = 0xEFE7E7E7;

    data().title_bar.inactive.background.color = 0xEFF0F0F0;

    data().title_bar.active.background.shaded = true;
    data().title_bar.active.background.shaded_count = 2;
    data().title_bar.active.background.shaded_colors.resize(2);
    data().title_bar.active.background.shaded_colors[0] = 0xFFDDDDDD;
    data().title_bar.active.background.shaded_colors[1] = 0xFFCCCCCC;
    data().title_bar.active.background.shaded_positions.resize(2);
    data().title_bar.active.background.shaded_positions[0] = 0.f;
    data().title_bar.active.background.shaded_positions[1] = 1.f;

    data().title_bar.highlight.background.color = data().title_bar.active.background.color + 55;
    data().title_bar.inactive.foreground.color = 0xFF444444;
    data().title_bar.active.foreground.color = 0xFF444444;
    data().title_bar.highlight.foreground.color = data().title_bar.active.foreground.color + 55;

    data().button.inactive.outline.color = 0xEF999999;
    data().button.inactive.background.color = 0xFFE0E0E0;
    data().button.inactive.background.shaded = true;
    data().button.inactive.background.shaded_count = 2;
    data().button.inactive.background.shaded_colors.resize(2);
    data().button.inactive.background.shaded_colors[0] = 0xFFDDDDDD;
    data().button.inactive.background.shaded_colors[1] = 0xFFCCCCCC;
    data().button.inactive.background.shaded_positions.resize(2);
    data().button.inactive.background.shaded_positions[0] = 0.f;
    data().button.inactive.background.shaded_positions[1] = 1.f;
    data().button.inactive.foreground.color = 0xFF444444;
    data().button.active.background.color = 0xFFB67E3E;
    data().button.active.background.shaded = true;
    data().button.active.background.shaded_count = 2;
    data().button.active.background.shaded_colors.resize(2);
    data().button.active.background.shaded_colors[0] = 0xFFAAAAAA;
    data().button.active.background.shaded_colors[1] = 0xFFBBBBBB;
    data().button.active.background.shaded_positions.resize(2);
    data().button.active.background.shaded_positions[0] = 0.f;
    data().button.active.background.shaded_positions[1] = 1.f;
    data().button.active.foreground.color = 0xFFEEEEEE;
    data().button.highlight.background.color = data().button.inactive.background.color + 25;
    data().button.highlight.background.shaded = true;
    data().button.highlight.background.shaded_count = 2;
    data().button.highlight.background.shaded_colors.resize(2);
    data().button.highlight.background.shaded_colors[0] = 0xFFE7E7E7;
    data().button.highlight.background.shaded_colors[1] = 0xFFD5D5D5;
    data().button.highlight.background.shaded_positions.resize(2);
    data().button.highlight.background.shaded_positions[0] = 0.f;
    data().button.highlight.background.shaded_positions[1] = 1.f;
    data().button.highlight.foreground.color = data().button.active.foreground.color + 25;
  }

  ~ThemeLight() {}

};

}
}

void *ThemeLightCreate() {
  return new skland::gui::ThemeLight;
}

void ThemeLightDestroy(void *p) {
  skland::gui::ThemeLight *theme = static_cast<skland::gui::ThemeLight *>(p);
  delete theme;
}

