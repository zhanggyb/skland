//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/rect.hpp>

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, constructor_1) {
  Rect r;

  ASSERT_TRUE(r.x() == 0 && r.y() == 0 && r.width() == 0 && r.height() == 0);
}

TEST_F(Test, constructor_2) {
  Rect r(0, 0, 4, 4);

  ASSERT_TRUE(r.x() == 0 && r.y() == 0 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, set_size_1) {
  Rect r;

  r.Resize(4, 4);

  ASSERT_TRUE(r.x() == 0 && r.y() == 0 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, set_position_1) {
  Rect r(0, 0, 4, 4);

  r.MoveTo(1, 1);

  ASSERT_TRUE(r.x() == 1 && r.y() == 1 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, set_position_2) {
  Rect r(0, 0, 4, 4);

  r.MoveTo(1, 1);

  ASSERT_TRUE(r.x() == 1 && r.y() == 1 && r.width() == 4 && r.height() == 4);
}

TEST_F(Test, contains_1) {
  Rect r(0, 0, 4, 4);

  ASSERT_TRUE(r.Contain(1, 1));
}

TEST_F(Test, contains_2) {
  Rect r(0, 0, 4, 4);

  ASSERT_TRUE(r.Contain(0, 0));
}

TEST_F(Test, contains_3) {
  Rect r(0, 0, 4, 4);

  ASSERT_TRUE(r.Contain(4, 4));
}

TEST_F(Test, hcenter_1) {
  Rect r(0, 0, 4, 4);

  ASSERT_TRUE(r.center_x() == 2);
}

TEST_F(Test, vcenter_1) {
  Rect r(0, 0, 4, 4);

  ASSERT_TRUE(r.center_y() == 2);
}
