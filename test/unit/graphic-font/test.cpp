//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/color.hpp>
#include <skland/graphic/font.hpp>

#include "SkTypeface.h"

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, enums_check_1) {

  // Compare Font::Weight and SkFontStyle::Weight
  ASSERT_TRUE((int) Font::Weight::kWeightInvisible == (int) SkFontStyle::Weight::kInvisible_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightThin == (int) SkFontStyle::Weight::kThin_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightExtraLight == (int) SkFontStyle::Weight::kExtraLight_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightLight == (int) SkFontStyle::Weight::kLight_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightNormal == (int) SkFontStyle::Weight::kNormal_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightMedium == (int) SkFontStyle::Weight::kMedium_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightSemiBold == (int) SkFontStyle::Weight::kSemiBold_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightBold == (int) SkFontStyle::Weight::kBold_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightExtraBold == (int) SkFontStyle::Weight::kExtraBold_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightBlack == (int) SkFontStyle::Weight::kBlack_Weight);
  ASSERT_TRUE((int) Font::Weight::kWeightExtraBlack == (int) SkFontStyle::Weight::kExtraBlack_Weight);

  // Compare Font::Width and SkFontStyle::Width
  ASSERT_TRUE((int) Font::Width::kWidthUltraCondensed == (int) SkFontStyle::kUltraCondensed_Width);
  ASSERT_TRUE((int) Font::Width::kWidthExtraCondensed == (int) SkFontStyle::kExtraCondensed_Width);
  ASSERT_TRUE((int) Font::Width::kWidthCondensed == (int) SkFontStyle::kCondensed_Width);
  ASSERT_TRUE((int) Font::Width::kWidthSemiCondensed == (int) SkFontStyle::kSemiCondensed_Width);
  ASSERT_TRUE((int) Font::Width::kWidthNormal == (int) SkFontStyle::kNormal_Width);
  ASSERT_TRUE((int) Font::Width::kWidthSemiExpanded == (int) SkFontStyle::kSemiExpanded_Width);
  ASSERT_TRUE((int) Font::Width::kWidthExpanded == (int) SkFontStyle::kExpanded_Width);
  ASSERT_TRUE((int) Font::Width::kWidthExtraExpanded == (int) SkFontStyle::kExtraExpanded_Width);
  ASSERT_TRUE((int) Font::Width::kWidthUltraExpanded == (int) SkFontStyle::kUltraExpanded_Width);

  // Compare Font::Slant and SkFontStyle::Slant
  ASSERT_TRUE((int) Font::Slant::kSlantUpright == (int) SkFontStyle::kUpright_Slant);
  ASSERT_TRUE((int) Font::Slant::kSlantItalic == (int) SkFontStyle::kItalic_Slant);
  ASSERT_TRUE((int) Font::Slant::kSlantOblique == (int) SkFontStyle::kOblique_Slant);

}
