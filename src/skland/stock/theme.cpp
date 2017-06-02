/*
 * Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <skland/stock/theme.hpp>
#include <iostream>
#include <skland/core/assert.hpp>
#include <skland/skland.hpp>
#include <skland/graphic/gradient-shader.hpp>

#include "SkBlurMaskFilter.h"
#include "SkPath.h"
#include "SkCanvas.h"
#include "SkPixmap.h"

#include "internal/theme-light.hpp"
#include "internal/theme-dark.hpp"

namespace skland {

int Theme::kShadowRadius = 33;
int Theme::kShadowOffsetX = 0;
int Theme::kShadowOffsetY = 11;
Margin Theme::kShadowMargin = Margin(kShadowRadius - kShadowOffsetX,
                                     kShadowRadius - kShadowOffsetY,
                                     kShadowRadius + kShadowOffsetX,
                                     kShadowRadius + kShadowOffsetY);
std::vector<uint32_t> Theme::kShadowPixels;
SkPixmap *Theme::kShadowPixmap = nullptr;

Theme *Theme::kTheme = nullptr;

Theme::Data::Data()
    : title_bar_font("Noto Sans CJK SC",
                     FontStyle(FontStyle::kWeightBold, FontStyle::kWidthNormal, FontStyle::kSlantUpright),
                     12.f),
      default_font("Noto Sans CJK SC",
                   FontStyle(FontStyle::kWeightNormal, FontStyle::kWidthNormal, FontStyle::kSlantUpright),
                   12.f) {

}

Shader Theme::Helper::GradientShader::MakeLinear(const Point2F *points, const Schema::ShadedColor &color) {
  return skland::GradientShader::MakeLinear(points,
                                            color.shaded_colors.data(),
                                            color.shaded_positions.data(),
                                            color.shaded_count,
                                            Shader::TileMode::kTileModeClamp);
}

void Theme::Initialize() {
  if (kTheme) return;

  kShadowPixels.resize(kShadowImageWidth * kShadowImageHeight, 0);
  GenerateShadowImage();

  SkImageInfo image_info = SkImageInfo::MakeN32Premul(kShadowImageWidth, kShadowImageHeight);
  delete kShadowPixmap;
  kShadowPixmap = new SkPixmap(image_info,
                               kShadowPixels.data(),
                               kShadowImageWidth * 4);

  Load();
  _ASSERT(kTheme);
}

void Theme::Release() {
  if (nullptr == kTheme) return;

  delete kTheme;
  kTheme = nullptr;

  delete kShadowPixmap;
  kShadowPixmap = nullptr;
}

Theme::Theme() {

}

Theme::~Theme() {

}

void Theme::Load(const char *name) {
  if (kTheme) delete kTheme;
  kTheme = nullptr;

  if (nullptr == name) name = "Light";

  ThemeCreateHandle handle = nullptr;
  // TODO: load from file

  // Otherwise, use the builtin theme
  std::string upper_name(name);
  std::transform(upper_name.begin(), upper_name.end(), upper_name.begin(), ::toupper);

  if (upper_name == "LIGHT") {
    handle = ThemeLightCreate;
    kTheme = static_cast<Theme *>(handle());
    kTheme->data_.name = name;
  } else if (upper_name == "DARK") {
    handle = ThemeDarkCreate;
    kTheme = static_cast<Theme *>(handle());
  }
}

void Theme::GenerateShadowImage() {
  std::unique_ptr<SkCanvas> canvas =
      SkCanvas::MakeRasterDirectN32(kShadowImageWidth,
                                    kShadowImageHeight,
                                    kShadowPixels.data(),
                                    kShadowImageWidth * 4);
  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setARGB(135, 0, 0, 0);
  paint.setMaskFilter(SkBlurMaskFilter::Make(
      kNormal_SkBlurStyle, kShadowRadius / 2.f - 0.5f, 0x2));  // Use high-quality blur

  float radii[] = {
      kShadowRadius / 1.f, kShadowRadius / 1.f, // top-left
      kShadowRadius / 1.f, kShadowRadius / 1.f, // top-right
      kShadowRadius / 2.f, kShadowRadius / 2.f, // bottom-right
      kShadowRadius / 2.f, kShadowRadius / 2.f,  // bottom-left
  };

  SkPath path;
  path.addRoundRect(SkRect::MakeLTRB(kShadowRadius,
                                     kShadowRadius,
                                     kShadowImageWidth - kShadowRadius,
                                     kShadowImageWidth - kShadowRadius),
                    radii, SkPath::kCW_Direction);
  canvas->drawPath(path, paint);
}

}
