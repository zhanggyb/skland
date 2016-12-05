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

#include <skland/stock/theme.hpp>

namespace skland {

Theme *Theme::kTheme = nullptr;

Theme::Theme() {
  Reset();
}

Theme::~Theme() {
}

void Theme::Reset() {
  window_frame_.outline = 0x303030;
  window_frame_.inner = 0x414141FF;
  window_frame_.inner_selected = 0xB67E3EFF;
  window_frame_.item = 0x191919FF;
  window_frame_.text = 0xB8B8B8;
  window_frame_.text_selected = 0xFFFFFF;
  window_frame_.shaded = true;
  window_frame_.shadetop = 15;
  window_frame_.shadedown = 0;
}

}
