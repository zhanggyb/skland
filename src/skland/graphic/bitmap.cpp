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

#include "internal/bitmap_private.hpp"
#include "internal/image-info_private.hpp"

#include "skland/core/memory.hpp"

#include "OpenImageIO/imageio.h"

namespace skland {
namespace graphic {

Bitmap::Bitmap() {
  p_ = core::make_unique<Private>(this);
}

Bitmap::Bitmap(const Bitmap &orig) {
  p_ = core::make_unique<Private>(this);

  p_->sk_bitmap = orig.p_->sk_bitmap;
}

Bitmap &Bitmap::operator=(const Bitmap &other) {
  p_->sk_bitmap = other.p_->sk_bitmap;
  return *this;
}

Bitmap::~Bitmap() {

}

bool Bitmap::InstallPixels(const ImageInfo &info, void *pixels, size_t row_bytes) {
  return p_->sk_bitmap.installPixels(SkImageInfo::Make(info.width(),
                                                       info.height(),
                                                       static_cast<SkColorType >(info.color_type()),
                                                       static_cast<SkAlphaType >(info.alpha_type())),
                                     pixels,
                                     row_bytes);
}

void Bitmap::WriteToFile(const std::string &filename) const {
  using OIIO::ImageOutput;
  using OIIO::ImageSpec;
  using OIIO::TypeDesc;

  ImageOutput *out = ImageOutput::create(filename);
  if (nullptr == out) return;

  ImageSpec spec (p_->sk_bitmap.width(), p_->sk_bitmap.height(), 4, TypeDesc::UINT8);
  out->open(filename, spec);
  out->write_image(TypeDesc::UINT8, p_->sk_bitmap.getPixels());
  out->close();
  ImageOutput::destroy(out);
}

int Bitmap::GetWidth() const {
  return p_->sk_bitmap.width();
}

int Bitmap::GetHeight() const {
  return p_->sk_bitmap.height();
}

ColorType Bitmap::GetColorType() const {
  return static_cast<ColorType>(p_->sk_bitmap.colorType());
}

AlphaType Bitmap::GetAlphaType() const {
  return static_cast<AlphaType>(p_->sk_bitmap.alphaType());
}

} // namespace graphic
} // namespace skland
