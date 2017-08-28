//
// Created by zhanggyb on 16-9-19.
//

#ifndef WAYLAND_TOOLKIT_TEST_HPP
#define WAYLAND_TOOLKIT_TEST_HPP

#include <gtest/gtest.h>

class MatrixTest : public testing::Test {
 public:
  MatrixTest();
  virtual ~MatrixTest();

 protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

#endif //WAYLAND_TOOLKIT_TEST_HPP
