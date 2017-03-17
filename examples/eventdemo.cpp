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

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>
#include <skland/gui/abstract-view.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include <iostream>
#include <skland/gui/push-button.hpp>

using namespace skland;

class SimpleWidget : public AbstractView {

 public:

  SimpleWidget()
      : AbstractView(), btn_(nullptr) {
    btn_ = new PushButton("Click me");
    btn_->MoveTo(200, 200);
    btn_->Resize(200, 200);

    InsertChild(btn_);
  }

  virtual ~SimpleWidget() {
//    btn_->Destroy();  // btn_ is destroyed before this destructor
  }

 protected:

  virtual void OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override {
    if (dirty_flag) Update();
    else CancelUpdate();
  }

  virtual void OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override {

  }

  virtual void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseLeave() override {

  }

  virtual void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseButton(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnKeyboardKey(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnDraw(const Context *context) override {
//    Paint paint;
//    paint.SetColor(Color(0.055f, 0.125f, 0.165f, 1.f));

//    canvas->DrawRectangle(x(), y(), width(), height(), paint);
//    canvas->Flush();
  }

 private:

  PushButton *btn_;

};

int main(int argc, char *argv[]) {
  using skland::Window;

  Application app(argc, argv);

  std::cout << sizeof(SimpleWidget) << std::endl;

  Window *win = new Window(800, 600, "Event Demo");
  win->SetAppId("Event-Demo");

  SimpleWidget *widget = new SimpleWidget;
  win->SetContentView(widget);

  win->Show();

  return app.Run();
}
