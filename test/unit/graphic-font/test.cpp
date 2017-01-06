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
  ASSERT_TRUE(TextEncoding::kTextEncodingUTF8 == SkTextEncoding::kUTF8_SkTextEncoding);
  ASSERT_TRUE(TextEncoding::kTextEncodingUTF16 == SkTextEncoding::kUTF16_SkTextEncoding);
  ASSERT_TRUE(TextEncoding::kTextEncodingUTF32 == SkTextEncoding::kUTF32_SkTextEncoding);
  ASSERT_TRUE(TextEncoding::kTextEncodingGlyphID == SkTextEncoding::kGlyphID_SkTextEncoding);

  ASSERT_TRUE(Font::Flags::kFlagEnableAutoHints == SkFont::Flags::kEnableAutoHints_Flag);
  ASSERT_TRUE(Font::Flags::kFlagEnableByteCodeHints == SkFont::Flags::kEnableByteCodeHints_Flag);
  ASSERT_TRUE(Font::Flags::kFlagEmbeddedBitmaps == SkFont::Flags::kEmbeddedBitmaps_Flag;
  ASSERT_TRUE(Font::Flags::kFlagUseNonlinearMetrics == SkFont::Flags::kUseNonlinearMetrics_Flag);
  ASSERT_TRUE(Font::Flags::kFlagVertical == SkFont::Flags::kVertical_Flag);
  ASSERT_TRUE(Font::Flags::kFlagGenA8FromLCD == SkFont::Flags::kGenA8FromLCD_Flag);
  ASSERT_TRUE(Font::Flags::kFlagEmbolden == SkFont::Flags::kEmbolden_Flag);
  ASSERT_TRUE(Font::Flags::kFlagDevKern == SkFont::Flags::kDevKern_Flag);

  ASSERT_TRUE(Font::MaskType::kMaskBW == SkFont::MaskType::kBW_MaskType);
  ASSERT_TRUE(Font::MaskType::kMaskA8 == SkFont::MaskType::kA8_MaskType);
  ASSERT_TRUE(Font::MaskType::kMaskLCD == SkFont::MaskType::kLCD_MaskType);
}
