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

#ifndef SKLAND_GRAPHIC_IMAGE_INFO_HPP_
#define SKLAND_GRAPHIC_IMAGE_INFO_HPP_

#include <memory>

// Forward declaration
struct SkImageInfo;

namespace skland {

enum AlphaType {
  kAlphaTypeUnknown,

  kAlphaTypeOpaque,

  kAlphaTypePremul,

  kAlphaTypeUnpremul,

  kAlphaTypeLast = kAlphaTypeUnpremul
};

enum ColorType {
  kColorTypeUnknown,

  kColorTypeAlpha8,
  kColorTypeRGB565,
  kColorTypeARGB4444,
  kColorTypeRGBA8888,
  kColorTypeBGRA8888,
  kColorTypeIndex8,
  kColorTypeGray8,
  kColorTypeRGBAF16,

  kColorTypeLast = kColorTypeRGBAF16
};

class ImageInfo {

  friend class Bitmap;
  friend class Layer;
  friend class Canvas;

 public:

  static ImageInfo Make(int width, int height, ColorType ct, AlphaType at);

  static ImageInfo MakeN32Premul(int width, int height);

  ImageInfo();

  ImageInfo(const ImageInfo &orig);

  ~ImageInfo();

  ImageInfo &operator=(const ImageInfo &other);

 private:

  ImageInfo(const SkImageInfo &native);

  struct Private;

  std::unique_ptr<Private> p_;

};

}

#endif //SKLAND_IMAGE_INFO_HPP
