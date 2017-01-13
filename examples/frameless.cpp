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
#include <skland/gui/abstract-widget.hpp>

#include <skland/gui/context.hpp>
#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

using namespace skland;

class MainWidget : public AbstractWidget {

 public:

  MainWidget()
      : AbstractWidget() {
  }

  virtual ~MainWidget() {}

 protected:

  virtual void OnResize(int width, int height) override {
    resize(width, height);
    Update();
  }

  virtual void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseLeave(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseButton(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyboardKey(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnDraw(const Context *context) override {
    Paint paint;
    paint.SetColor(Color(0.855f, 0.855f, 0.165f, 1.f));

    context->GetCanvas()->DrawRect(geometry(), paint);
  }

};

int main(int argc, char *argv[]) {
  using skland::Window;

  Application app(argc, argv);

  Window *win = new Window(400, 360, "Frameless Window", nullptr);
  win->SetAppId("Frameless-Demo");

  MainWidget *widget = new MainWidget;
  win->SetMainWidget(widget);

  win->Show();

  return app.Run();
}
