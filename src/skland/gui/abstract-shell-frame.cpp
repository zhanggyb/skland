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

#include "internal/abstract-shell-frame-private.hpp"

#include <skland/gui/abstract-view.hpp>
#include "internal/abstract-shell-view-private.hpp"

namespace skland {

const Margin AbstractShellFrame::kResizingMargin(5, 5, 5, 5);

AbstractShellFrame::AbstractShellFrame()
    : Trackable() {
  p_.reset(new Private);
}

AbstractShellFrame::~AbstractShellFrame() {
  if (p_->shell_view) {
    p_->shell_view->p_->shell_frame = nullptr;
  }

  SetTitleView(nullptr);
  // TODO: update shell_view_
}

void AbstractShellFrame::SetTitleView(AbstractView *view) {
  if (p_->title_view == view) return;

  if (p_->title_view) {
    p_->title_view->Destroy();
  }

  p_->title_view = view;
  if (p_->title_view) {
    if (p_->shell_view) {
      p_->shell_view->AttachView(p_->title_view);
    }
  }
}

AbstractView *AbstractShellFrame::GetTitleView() const {
  return p_->title_view;
}

AbstractShellView *AbstractShellFrame::GetShellView() const {
  return p_->shell_view;
}

}