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

#ifndef SKLAND_CORE_RECT_HPP_
#define SKLAND_CORE_RECT_HPP_

#include <cstring>
#include "point.hpp"
#include "size.hpp"

namespace skland {

namespace core {

/**
 * @ingroup core
 * @brief Template class for rectangle with 4 edges (left, top, right, bottom).
 */
template<typename T>
struct Rect {

  inline static Rect FromLTRB(T left, T top, T right, T bottom) {
    return Rect(left, top, right, bottom);
  }

  inline static Rect FromXYWH(T x, T y, T width, T height) {
    return Rect(x, y, x + width, y + height);
  }

  template<typename R>
  inline static Rect FromOther(const Rect<R> &other) {
    return Rect(T(other.left), T(other.top), T(other.right), T(other.bottom));
  }

  inline Rect()
      : left(0), top(0), right(0), bottom(0) {}

  inline Rect(T left, T top, T right, T bottom)
      : left(left), top(top), right(right), bottom(bottom) {}

  inline Rect(T width, T height)
      : left(0), top(0), right(width), bottom(height) {}

  inline Rect(const Rect &other)
      : left(other.left), top(other.top), right(other.right), bottom(other.bottom) {}

  template<typename R>
  inline Rect(const Rect<R> &other)
      : left(T(other.left)), top(T(other.top)), right(T(other.right)), bottom(T(other.bottom)) {}

  inline Rect &operator=(const Rect &other) {
    left = other.left;
    top = other.top;
    right = other.right;
    bottom = other.bottom;
    return *this;
  }

  template<typename R>
  inline Rect &operator=(const Rect<R> &other) {
    left = T(other.left);
    top = T(other.top);
    right = T(other.right);
    bottom = T(other.bottom);
    return *this;
  }

  inline bool Contain(T x, T y) const {
    return left <= x && x < right && top <= y && y < bottom;
  }

  inline void MoveTo(T x, T y) {
    right = x + width();
    bottom = y + height();
    left = x;
    top = y;
  }

  inline void Move(T dx, T dy) {
    left += dx;
    top += dy;
    right += dx;
    bottom += dy;
  }

  inline void Resize(T width, T height) {
    right = left + width;
    bottom = top + height;
  }

  inline Rect Shrink(T value) const {
    return Rect(this->left + value, this->top + value, this->right - value, this->bottom - value);
  }

  inline Rect Expand(T value) const {
    return Rect(this->left - value, this->top - value, this->right + value, this->bottom + value);
  }

  inline T x() const { return left; }

  inline T y() const { return top; }

  inline T width() const { return right - left; }

  inline T height() const { return bottom - top; }

  inline T center_x() const { return left + width() / T(2.0); }

  inline T center_y() const { return top + height() / T(2.0); }

  union { T l, left; };
  union { T t, top; };
  union { T r, right; };
  union { T b, bottom; };

};

template<typename T>
inline Rect<T> operator*(const Rect<T> &src, int factor) {
  return Rect<T>(src.left * factor, src.top * factor, src.right * factor, src.bottom * factor);
}

/**
 * @ingroup core
 */
typedef Rect<int> RectI;

/**
 * @ingroup core
 */
typedef Rect<float> RectF;

/**
 * @ingroup core
 */
typedef Rect<double> RectD;

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_RECT_HPP_
