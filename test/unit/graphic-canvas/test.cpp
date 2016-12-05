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

  std::vector<uint32_t> image(100 * 100, 0);

  Canvas canvas;
  canvas.Setup((unsigned char*)image.data(), 100, 100);
  bool result1 = canvas.is_valid();
  canvas.Destroy();
  bool result2 = canvas.is_null();

  ASSERT_TRUE(result1 && result2);
}
