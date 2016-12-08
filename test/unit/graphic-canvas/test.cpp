//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/color.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/canvas.hpp>

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {

  ASSERT_TRUE(true);
}
