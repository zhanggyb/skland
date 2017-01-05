//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/graphic/font.hpp>
#include "SkFont.h"

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {
  ASSERT_TRUE(true);
}
