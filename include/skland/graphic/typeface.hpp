//
// Created by zhanggyb on 17-1-5.
//

#ifndef SKLAND_GRAPHIC_TYPEFACE_HPP_
#define SKLAND_GRAPHIC_TYPEFACE_HPP_

#include "font-style.hpp"

#include <memory>

namespace skland {

class Font;
struct TypefaceMeta;

class Typeface {

  friend class Font;

 public:

  enum Style {
    kNormal = 0,
    kBold = 0x01,
    kItalic = 0x02,

    kBoldItalic = 0x03
  };

  Typeface(Style style = kNormal);

  Typeface(const char *family_name, FontStyle font_style);

  Typeface(const Typeface &other, Style style);

  /**
   * Create a new typeface by given file.
   * @param path
   * @param index
   */
  Typeface(const char *path, int index = 0);

  ~Typeface();

  Typeface &operator=(const Typeface &other);

 private:

  std::unique_ptr<TypefaceMeta> metadata_;

};

}

#endif // SKLAND_GRAPHIC_TYPEFACE_HPP_
