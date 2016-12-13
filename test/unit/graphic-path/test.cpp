//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/graphic/path.hpp>
#include "SkPath.h"

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {

  ASSERT_TRUE((int) Path::Direction::kClockwise == (int) SkPath::Direction::kCW_Direction);
  ASSERT_TRUE((int) Path::Direction::kCounterClockwise == (int) SkPath::Direction::kCCW_Direction);

}
