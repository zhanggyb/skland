//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/color.hpp>
#include <skland/graphic/paint.hpp>
#include <SkPaint.h>

using skland::Color;
using skland::graphic::Paint;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {

  // Compare Paint::Style and SkPaint::Style
  ASSERT_TRUE((int) Paint::Style::kStyleFill == (int) SkPaint::Style::kFill_Style);
  ASSERT_TRUE((int) Paint::Style::kStyleStroke == (int) SkPaint::Style::kStroke_Style);
  ASSERT_TRUE((int) Paint::Style::kStyleStrokeAndFill == (int) SkPaint::Style::kStrokeAndFill_Style);

  // Compare Paint::Hinting and SkPaint::Hinting
  ASSERT_TRUE((int) Paint::Hinting::kHintingNone == (int) SkPaint::Hinting::kNo_Hinting);
  ASSERT_TRUE((int) Paint::Hinting::kHintingSlight == (int) SkPaint::Hinting::kSlight_Hinting);
  ASSERT_TRUE((int) Paint::Hinting::kHintingNormal == (int) SkPaint::Hinting::kNormal_Hinting);
  ASSERT_TRUE((int) Paint::Hinting::kHintingFull == (int) SkPaint::Hinting::kFull_Hinting);

}

TEST_F(Test, constructor_1) {
  Color blue(0.f, 0.f, 1.f, 1.f);
  Color ret;

  Paint paint;
  paint.SetColor(blue);
  ret = paint.GetColor();

  ASSERT_TRUE(ret == blue);
}
