//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/wayland/display.hpp>

using namespace skland::wayland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, connect_1) {
  Display display;
  display.Connect(NULL);

  ASSERT_TRUE(display.IsValid());
}

TEST_F(Test, disconnect_1) {
  Display display;
  display.Connect(NULL);
  display.Disconnect();

  ASSERT_TRUE(!display.IsValid());
}
