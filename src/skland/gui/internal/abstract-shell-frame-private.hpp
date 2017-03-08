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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_FRAME_PRIVATE_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_FRAME_PRIVATE_HPP_

#include <skland/gui/abstract-shell-frame.hpp>

namespace skland {

SKLAND_NO_EXPORT struct AbstractShellFrame::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : shell_view(nullptr),
        title_view(nullptr) {}

  ~Private() {}

  AbstractShellView *shell_view;

  AbstractView *title_view;

};

}

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_FRAME_PRIVATE_HPP_