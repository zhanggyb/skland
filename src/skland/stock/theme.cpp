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
#include <SkPath.h>

#include "SkCanvas.h"
#include "SkPaint.h"
#include "SkPixmap.h"

#include "internal/default-window-frame.hpp"

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

AbstractWindowFrame *Theme::CreateWindowFrame(bool dark) {
  return new DefaultWindowFrame(dark ? DefaultWindowFrame::kModeDark : DefaultWindowFrame::kModeLight);
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

  shadow_radius_ = 33;

  shadow_offset_x_ = 0;
  shadow_offset_y_ = 11;

  shadow_margin_.left = shadow_radius_ - shadow_offset_x_;
  shadow_margin_.right = shadow_radius_ + shadow_offset_x_;
  shadow_margin_.top = shadow_radius_ - shadow_offset_y_;
  shadow_margin_.bottom = shadow_radius_ + shadow_offset_y_;

  shadow_pixels_.resize(kShadowImageWidth * kShadowImageHeight, 0);
  GenerateShadowImage();

  SkImageInfo image_info = SkImageInfo::MakeN32Premul(kShadowImageWidth, kShadowImageHeight);
  delete shadow_pixmap_;
  shadow_pixmap_ = new SkPixmap(image_info,
                                shadow_pixels_.data(),
                                kShadowImageWidth * 4);
}

void Theme::GenerateShadowImage() {
  std::unique_ptr<SkCanvas> canvas =
      SkCanvas::MakeRasterDirectN32(kShadowImageWidth,
                                    kShadowImageHeight,
                                    shadow_pixels_.data(),
                                    kShadowImageWidth * 4);
  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setARGB(105, 0, 0, 0);
  paint.setMaskFilter(SkBlurMaskFilter::Make(
      kNormal_SkBlurStyle, shadow_radius_ / 2.f - 0.5f, 0x2));  // Use high-quality blur

  float radii[] = {
      shadow_radius_ / 1.f, shadow_radius_ / 1.f, // top-left
      shadow_radius_ / 1.f, shadow_radius_ / 1.f, // top-right
      shadow_radius_ / 2.f, shadow_radius_ / 2.f, // bottom-right
      shadow_radius_ / 2.f, shadow_radius_ / 2.f,  // bottom-left
  };

  SkPath path;
  path.addRoundRect(SkRect::MakeLTRB(shadow_radius_,
                                     shadow_radius_,
                                     kShadowImageWidth - shadow_radius_,
                                     kShadowImageWidth - shadow_radius_),
                    radii, SkPath::kCW_Direction);
  canvas->drawPath(path, paint);
}

}
