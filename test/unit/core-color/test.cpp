//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/color.hpp>

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {
  ColorD c;

  ASSERT_TRUE(c.r == 0.0 && c.g == 0.0 && c.b == 0.0 && c.a == 0.0);
}

TEST_F(Test, constructor_2) {
  ColorD c(0xFF00FF);

  ASSERT_TRUE(c.r == 1.0 && c.g == 0.0 && c.b == 1.0 && c.a == 0.0);
}

TEST_F(Test, assign_1) {
  ColorD c;
  c = 0xFF00FF00;
  ASSERT_TRUE(c.r == 0.0 && c.g == 1.0 && c.b == 0.0 && c.a == 1.0);
}

TEST_F(Test, assign_2) {
  ColorD c1;
  c1 = 0xFF00FF00;
  ColorD c;
  c = c1;

  ASSERT_TRUE(c.r == 0.0 && c.g == 1.0 && c.b == 0.0 && c.a == 1.0);
}

TEST_F(Test, compare_1) {
  ColorD c1(0xFF);
  ColorD c2(0xFF);

  ASSERT_TRUE(c1 == c2);
}

TEST_F(Test, compare_2) {
  ColorD c1(0.4, 0.6, 0.2, 0.9);
  ColorD c2(0.8, 0.2, 0.5, 0.7);

  ASSERT_TRUE(c1 != c2);
}

TEST_F(Test, argb_1) {
  uint32_t argb = 0xFFEEDDCC;
  ColorD c(argb);

  ASSERT_TRUE(argb = c.argb());
}
