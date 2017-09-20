//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/display.hpp>

#include <iostream>

using skland::gui::Application;
using skland::gui::Display;
using std::cout;
using std::endl;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, application) {
  using skland::gui::Display;

  int argc = 1;
  char argv1[] = "application";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);
  app.Exit();

  ASSERT_TRUE(true);
}
