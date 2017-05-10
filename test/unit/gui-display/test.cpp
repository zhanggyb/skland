//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/display.hpp>

#include <iostream>

using skland::Application;
using skland::Display;
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
  using skland::Display;

  int argc = 1;
  char argv1[] = "application";  // to avoid compile warning
  char *argv[] = {argv1};

  bool result1 = (Application::Get() == nullptr);

  Application app(argc, argv);
  bool result2 = (Application::Get() != nullptr);

  Display *display = app.display();

  cout << "outputs count: " << display->GetOutputCount() << endl;
//  cout << "pixel format: " << display->pixel_formats() << endl;

  app.Exit();
  ASSERT_TRUE(result1 && result2);
}
