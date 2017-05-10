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

/*
 *
 */
TEST_F(Test, timer_1) {
  using skland::Window;

  int argc = 1;
  char argv1[] = "resize_1";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Timer t;
  t.SetInterval(1000);
  t.Start();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
