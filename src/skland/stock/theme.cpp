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
#include <SkBlurMaskFilter.h>

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkPixmap.h"

namespace skland {

Theme *Theme::kTheme = nullptr;

Theme::Theme()
    : shadow_pixmap_(nullptr) {
  shadow_pixmap_ = new SkPixmap;
  Reset();
}

Theme::~Theme() {
  delete shadow_pixmap_;
}

void Theme::Reset() {
  window_frame_.outline = 0x303030;
  window_frame_.inner = 0x414141FF;
  window_frame_.inner_selected = 0xB67E3EFF;
  window_frame_.item = 0x191919FF;
  window_frame_.text = 0xB8B8B8;
  window_frame_.text_selected = 0xFFFFFF;
  window_frame_.shaded = true;
  window_frame_.shadetop = 15;
  window_frame_.shadedown = 0;

  shadow_radius_ = 35;

  shadow_offset_x_ = 0;
  shadow_offset_y_ = 10;

  shadow_pixels_.resize(kShadowPixelsWidth * kShadowPixelsHeight, 0);
  GenerateShadowImage();

  SkImageInfo image_info = SkImageInfo::MakeN32Premul(kShadowPixelsWidth, kShadowPixelsHeight);
  delete shadow_pixmap_;
  shadow_pixmap_ = new SkPixmap(image_info,
                                shadow_pixels_.data(),
                                kShadowPixelsWidth * 4);
}

void Theme::GenerateShadowImage() {
  std::unique_ptr<SkCanvas> canvas =
      SkCanvas::MakeRasterDirectN32(kShadowPixelsWidth,
                                    kShadowPixelsHeight,
                                    shadow_pixels_.data(),
                                    kShadowPixelsWidth * 4);
  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setARGB(255, 0, 0, 0);
  paint.setMaskFilter(SkBlurMaskFilter::Make(
      kNormal_SkBlurStyle, 35 / 2.f - 0.5f, 0x2));

  canvas->drawRoundRect(SkRect::MakeLTRB(shadow_radius_,
                                    shadow_radius_,
                                    kShadowPixelsWidth - shadow_radius_,
                                    kShadowPixelsWidth - shadow_radius_),
                        35.f, 35.f,
                   paint);
}

}
