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

#include <skland/gui/abstract-window.hpp>
#include <skland/gui/application.hpp>
#include <skland/gui/surface.hpp>

namespace skland {

AbstractWindow::AbstractWindow(const char *title, int flags)
    : AbstractWindow(400, 300, title, flags) {
}

AbstractWindow::AbstractWindow(int width, int height, const char *title, int flags)
    : AbstractView(width, height),
      display_(nullptr),
      flags_(flags) {
  if (title) title_ = title;

  Display::AddWindow(this);
  // TODO: layout in display
}

AbstractWindow::~AbstractWindow() {
  if (display_)
    RemoveManagedObject(display_,
                        this,
                        &display_,
                        &display_->first_window_,
                        &display_->last_window_,
                        display_->windows_count_);
  DBG_ASSERT(display_ == nullptr);
}

void AbstractWindow::Close(SLOT) {

}

void AbstractWindow::Maximize(SLOT) {

}

void AbstractWindow::Minimize(SLOT) {

}

Size AbstractWindow::GetMinimalSize() const {
  return Size(160, 120);
}

Size AbstractWindow::GetPreferredSize() const {
  return Size(640, 480);
}

Size AbstractWindow::GetMaximalSize() const {
  return Size(65536, 65536);
}

void AbstractWindow::AddSubView(AbstractView *view, int pos) {
  if (view) InsertChild(view, pos);
}

}