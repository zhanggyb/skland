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

#include <skland/gui/main-window.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/output.hpp>
#include <skland/gui/raster-surface.hpp>
#include <skland/gui/abstract-widget.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/stock/theme.hpp>

namespace skland {

MainWindow::MainWindow(const char *title)
    : MainWindow(640, 480, title) {
}

MainWindow::MainWindow(int width, int height, const char *title)
    : AbstractWindow(width, height, title),
      main_widget_(nullptr) {
  set_name(title);  // debug only

  Size output_size(1024, 800);
  if (const Output *output = Display::GetOutputAt(0)) {
    output_size = output->current_mode_size();  // The current screen size
  }

  int total_width = std::max((int) (this->width()), output_size.width);
  int total_height = std::max((int) (this->height()), output_size.height);
  if (!IsFrameless()) {
    total_width += surface()->margin().lr();
    total_height += surface()->margin().tb();
  }

  pool_.Setup(total_width * 4 * total_height);
  buffer_.Setup(pool_, total_width, total_height, total_width * 4, WL_SHM_FORMAT_ARGB8888);
}

MainWindow::~MainWindow() {
  delete main_widget_;
}

void MainWindow::SetMainWidget(AbstractWidget *widget) {
  if (main_widget_) {
    delete main_widget_;
  }
  main_widget_ = widget;

  AddSubView(widget, -1);
  SetMainWidgetGeometry();
}

void MainWindow::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void MainWindow::OnResize(int width, int height) {
  resize(width, height);

  width += surface()->margin().lr();
  height += surface()->margin().tb();

  int total_size = width * 4 * height;
  if (total_size > pool_.size()) {
    DBG_PRINT_MSG("size_required: %d, pool size: %d, %s\n", total_size, pool_.size(), "Re-generate shm pool");
    pool_.Setup(total_size);
  }
  buffer_.Setup(pool_, width, height, width * 4, WL_SHM_FORMAT_ARGB8888);
  surface()->Attach(&buffer_);

  RedrawAll();

  SetMainWidgetGeometry();
}

void MainWindow::OnSetupSurface() {
  surface()->Attach(&buffer_);
  RedrawAll();
}

void MainWindow::SetMainWidgetGeometry() {
  if (main_widget_ == nullptr) return;

  Rect rect = GetClientGeometry();

  main_widget_->SetPosition(rect.x(), rect.y());
  main_widget_->Resize(rect.width(), rect.height());
}

}
