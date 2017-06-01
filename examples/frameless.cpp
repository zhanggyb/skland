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

#include <skland/gui/context.hpp>
#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

using namespace skland;

class FramelessWindow : public skland::Window {

 public:

  FramelessWindow(const char *title, int flags)
      : FramelessWindow(400, 300, title, flags) {

  }

  FramelessWindow(int width, int height, const char *title, int flags)
      : Window(width, height, title, flags) {}

  virtual ~FramelessWindow() {}

  virtual void OnMouseDown(MouseEvent *event) override {
    if (event->GetButton() == MouseButton::kMouseButtonLeft) {
      int location = GetMouseLocation(event);
      if (location == kClientArea) {
        MoveWithMouse(event);
        return;
      }
      if (location < kResizeMask) {
        ResizeWithMouse(event, (uint32_t) location);
        return;
      }
    }
  }

};

class MainWidget : public AbstractView {

 public:

  MainWidget()
      : AbstractView() {
  }

  virtual ~MainWidget() {}

 protected:

  virtual void OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override {
    if (dirty_flag) Update();
    else CancelUpdate();
  }

  virtual void OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override {

  }

  virtual void OnLayout(int dirty_flag, int left, int top, int right, int bottom) final {

  }

  virtual void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseLeave() override {

  }

  virtual void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseDown(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseUp(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyDown(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyUp(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnDraw(const Context *context) override {
    Paint paint;
    paint.SetColor(Color(0.855f, 0.855f, 0.165f, .9f));
    const Rect& rect = GetGeometry();
    int scale = 2;

    int x = rect.l;
    int y = rect.t;
    int w = x + rect.width() * scale;
    int h = y + rect.height() * scale;

    context->canvas()->DrawRect(Rect::FromXYWH(x, y, w, h), paint);
  }

};

int main(int argc, char *argv[]) {
  using skland::Window;

  Application app(argc, argv);

  FramelessWindow *win = new FramelessWindow("Frameless Window", Window::kFlagMaskFrameless);
  win->SetAppId("Frameless-Demo");

  MainWidget *widget = new MainWidget;
  win->SetContentView(widget);

  win->Show();

  return app.Run();
}
