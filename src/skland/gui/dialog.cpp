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

#include "skland/gui/dialog.hpp"

#include "skland/gui/shared-memory-pool.hpp"
#include "skland/gui/title-bar.hpp"
#include "skland/gui/buffer.hpp"

#include "skland/graphic/canvas.hpp"

namespace skland {
namespace gui {

using graphic::Canvas;

struct Dialog::Private {

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private()
      : flags(0),
        title_bar(nullptr),
        content_view(nullptr) {}

  ~Private() = default;

  int flags;

  SharedMemoryPool pool;

  Buffer buffer;
  std::unique_ptr<Canvas> canvas;

  TitleBar *title_bar;

  AbstractView *content_view;

};

Dialog::Dialog(const char *title, AbstractShellView *parent)
    : AbstractShellView(title, parent) {
  p_.reset(new Private);
}

Dialog::~Dialog() {
  if (nullptr != p_->content_view) p_->content_view->Destroy();
  if (nullptr != p_->title_bar) p_->title_bar->Destroy();
}

void Dialog::OnShown() {

}

bool Dialog::OnConfigureSize(const Size &old_size, const Size &new_size) {
  return true;
}

void Dialog::OnSizeChange(const Size &old_size, const Size &new_size) {

}

} // namespace gui
} // namespace skland
