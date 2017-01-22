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

#include <skland/gui/window.hpp>

#include <skland/gui/application.hpp>
#include <skland/gui/abstract-widget.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

namespace skland {

Window::Window(const char *title, AbstractWindowFrame *frame)
    : Window(640, 480, title, frame) {
}

Window::Window(int width, int height, const char *title, AbstractWindowFrame *frame)
    : AbstractWindow(width, height, title, frame),
      main_widget_(nullptr) {
}

Window::~Window() {
  delete main_widget_;
}

void Window::SetMainWidget(AbstractWidget *widget) {
  if (widget == main_widget_) return;

  if (main_widget_) delete main_widget_;
  main_widget_ = widget;

  AddSubView(widget);
  SetMainWidgetGeometry();
}

void Window::OnKeyboardKey(KeyEvent *event) {
  if (event->key() == kKey_ESC) {
    Application::Exit();
  }
  event->Accept();
}

void Window::OnResize(int /*width*/, int /*height*/) {
  SetMainWidgetGeometry();
}

void Window::SetMainWidgetGeometry() {
  if (main_widget_ == nullptr) return;

  Rect rect = GetClientGeometry();

  main_widget_->SetPosition((int) rect.x(), (int) rect.y());
  main_widget_->Resize((int) rect.width(), (int) rect.height());
}

}
