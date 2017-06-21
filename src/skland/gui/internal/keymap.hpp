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

#ifndef SKLAND_GUI_INTERNAL_KEYMAP_HPP_
#define SKLAND_GUI_INTERNAL_KEYMAP_HPP_

#include <xkbcommon/xkbcommon.h>

namespace skland {
namespace gui {

/**
 * @ingroup gui
 * @brief Wrapper class for XKB keymap object
 */
class Keymap {

  friend class KeyboardState;

 public:

  Keymap()
      : xkb_keymap_(nullptr) {}

  ~Keymap();

  /**
   * @brief Setup a XKB keymap from given string
   * @param string
   * @param format
   * @param flags
   */
  void Setup(const char *string, enum xkb_keymap_format format, enum xkb_keymap_compile_flags flags);

  void Destroy();

 private:

  struct xkb_keymap *xkb_keymap_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_KEYMAP_HPP_
