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

#ifndef SKLAND_GRAPHIC_BITMAP_HPP_
#define SKLAND_GRAPHIC_BITMAP_HPP_

#include <memory>
#include "image-info.hpp"

namespace skland {
namespace graphic {

/**
 * @ingroup graphic_intern
 * @brief A Bitmap class represents a raster bitmap.
 */
class Bitmap {

  friend class Canvas;

 public:

  Bitmap();

  Bitmap(const Bitmap &orig);

  Bitmap &operator=(const Bitmap &other);

  ~Bitmap();

  bool InstallPixels(const ImageInfo &info, void *pixels, size_t row_bytes);

  int GetWidth() const;

  int GetHeight() const;

  ColorType GetColorType() const;

  AlphaType GetAlphaType() const;

 private:

  struct Private;

  std::unique_ptr <Private> p_;

};

} // namespace graphic
} // namespace skland

#endif // SKLAND_GRAPHIC_BITMAP_HPP_
