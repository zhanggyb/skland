//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>

using skland::Application;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, application) {
  int argc = 1;
  char argv1[] = "application";  // to avoid compile warning
  char *argv[] = {argv1};

  bool result1 = (Application::instance() == nullptr);

  Application app(argc, argv);
  bool result2 = (Application::instance() != nullptr);

  app.Exit();

  ASSERT_TRUE(result1 && result2);
}
