//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>

#include <skland/gui/abstract-view.hpp>
#include <skland/gui/timer.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/graphic/paint.hpp>
#include <skland/graphic/canvas.hpp>

#include <skland/gui/context.hpp>

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

class ShmWidget : public AbstractView {

  Timer timer_;

 public:

  ShmWidget()
      : AbstractView() {
    timer_.SetInterval(1000); // 1s
    timer_.Start();
  }

  virtual ~ShmWidget() {}

 protected:

  virtual void OnSizeChanged(int width, int height) override {
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
    paint.SetColor(Color(0.055f, 0.125f, 0.165f, 1.f));

    context->canvas()->DrawRect(geometry(), paint);
  }

};

/*
 *
 */
TEST_F(Test, timer_1) {
  using skland::Window;

  int argc = 1;
  char argv1[] = "resize_1";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window *win = new Window(800, 600, "Simple");
//  ShmWidget* widget = new ShmWidget;
//  win->SetMainWidget(widget);

  win->Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
