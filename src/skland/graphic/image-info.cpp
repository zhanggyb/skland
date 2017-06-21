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

#include "internal/image-info_private.hpp"

namespace skland {
namespace graphic {

ImageInfo ImageInfo::Make(int width, int height, ColorType ct, AlphaType at) {
  SkImageInfo native = SkImageInfo::Make(width, height, (SkColorType) ct, (SkAlphaType) at);
  return ImageInfo(native);
}

ImageInfo ImageInfo::MakeN32Premul(int width, int height) {
  SkImageInfo native = SkImageInfo::MakeN32Premul(width, height);
  return ImageInfo(native);
}

ImageInfo::ImageInfo() {
  p_.reset(new Private);
}

ImageInfo::ImageInfo(const ImageInfo &orig) {
  p_.reset(new Private(*orig.p_));
}

ImageInfo::ImageInfo(const SkImageInfo &native) {
  p_.reset(new Private(native));
}

ImageInfo &ImageInfo::operator=(const ImageInfo &other) {
  *p_ = *other.p_;
  return *this;
}

ImageInfo::~ImageInfo() {

}

} // namespace graphic
} // namespace skland
