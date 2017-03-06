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

#ifndef SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_PRIVATE_HPP_
#define SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_PRIVATE_HPP_

#include <skland/gui/abstract-shell-view.hpp>

namespace skland {

struct AbstractShellView::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : flags_(0),
        shell_surface_(nullptr),
        parent_(nullptr),
        shell_frame_(nullptr),
        client_view_(nullptr) {}

  ~Private() {}

  int flags_;

  std::string title_;
  std::string app_id_;

  Size size_;

  Surface *shell_surface_;

  AbstractShellView *parent_;

  AbstractShellFrame *shell_frame_;

  AbstractView *client_view_;

};

} // namespace skland

#endif // SKLAND_GUI_INTERNAL_ABSTRACT_SHELL_VIEW_PRIVATE_HPP_
