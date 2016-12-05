//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/color.hpp>
#include <skland/graphic/paint.hpp>

using skland::Color;
using skland::Paint;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {
  Color blue(0.f, 0.f, 1.f, 1.f);
  Color ret;

  Paint paint;
  paint.SetColor(blue);
  ret = paint.GetColor();

  ASSERT_TRUE(ret == blue);
}
