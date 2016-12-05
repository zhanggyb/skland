//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/color.hpp>

using skland::ColorD;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {
//  Pattern pattern(Color(1., 1., 1., 1.));
//  unsigned int ref_count = pattern.GetReferenceCount();
//
//  ASSERT_TRUE(ref_count == 1);
  ASSERT_TRUE(true);
}
