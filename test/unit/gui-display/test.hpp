//
// Created by zhanggyb on 16-9-19.
//

#ifndef WAYLAND_TOOLKIT_TEST_HPP
#define WAYLAND_TOOLKIT_TEST_HPP

#include <gtest/gtest.h>

class Test : public testing::Test {
 public:
  Test();
  virtual ~Test();

 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

#endif //WAYLAND_TOOLKIT_TEST_HPP
