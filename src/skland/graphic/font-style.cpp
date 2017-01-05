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

#include <skland/graphic/font-style.hpp>

#include <skland/core/numeric.hpp>

namespace skland {

FontStyle::FontStyle() {
  data_.value = 0;
  data_.styles.weight = kWeightNormal;
  data_.styles.width = kWidthNormal;
  data_.styles.slant = kSlantUpright;
}

FontStyle::FontStyle(int weight, int width, Slant slant) {
  data_.value = 0;
  data_.styles.weight = (uint16_t) clamp<int>(weight, kWeightInvisible, kWeightExtraBlack);
  data_.styles.width = (uint8_t) clamp<int>(width, kWidthUltraCondensed, kWidthUltraExpanded);
  data_.styles.slant = (uint8_t) clamp<int>(slant, kSlantUpright, kSlantOblique);
}

}
