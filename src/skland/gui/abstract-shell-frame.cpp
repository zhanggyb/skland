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

#include <skland/gui/abstract-shell-frame.hpp>
#include <skland/gui/abstract-shell-view.hpp>
#include <skland/gui/push-button.hpp>

namespace skland {

const Margin AbstractShellFrame::kResizingMargin(5, 5, 5, 5);

AbstractShellFrame::AbstractShellFrame()
    : Trackable(),
      shell_view_(nullptr),
      title_bar_(nullptr) {

}

AbstractShellFrame::~AbstractShellFrame() {
  if (shell_view_) {
    shell_view_->shell_frame_ = nullptr;
    if (title_bar_) {
      shell_view_->DetachView(title_bar_);
    }
    // TODO: update shell_view_
  }

  title_bar_->Destroy();
}

void AbstractShellFrame::SetTitleBar(AbstractView *view) {
  if (title_bar_ == view) return;

  if (title_bar_) {
    title_bar_->Destroy();
  }

  title_bar_ = view;
  if (title_bar_) {
    if (shell_view_) {
      shell_view_->AttachView(title_bar_);
    }
  }
}

}
