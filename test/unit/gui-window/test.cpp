//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>

using skland::Application;
using skland::Window;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, resize_1) {
  using skland::Window;

  int argc = 1;
  char argv1[] = "resize_1";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Window *win = new Window(800, 600, "Simple");
  win->Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
