//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>
#include <skland/gui/egl-widget.hpp>

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, slider_1) {
  using skland::Window;

  int argc = 1;
  char argv1[] = "gui-slider";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window *win = new Window(480, 360, "Test EGL Widget");
  win->SetAppId("EGLWidget");

  EGLWidget *widget = new EGLWidget;
  win->SetMainWidget(widget);

  win->Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
