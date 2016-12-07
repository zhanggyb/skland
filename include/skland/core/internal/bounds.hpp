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

#ifndef SKLAND_CORE_INTERNAL_SIDE_SPACE_HPP_
#define SKLAND_CORE_INTERNAL_SIDE_SPACE_HPP_

namespace skland {
namespace core {

template<typename T>
struct Bounds {

  inline Bounds()
      : l(T(0)), t(T(0)), r(T(0)), b(T(0)) {}

  inline Bounds(T value)
      : l(value), t(value), r(value), b(value) {}

  inline Bounds(T l, T t, T r, T b)
      : l(l), t(t), r(r), b(b) {}

  inline Bounds(const Bounds &other)
      : l(other.l), t(other.t), r(other.r), b(other.b) {}

  inline ~Bounds() {}

  inline Bounds &operator=(const Bounds &other) {
    l = other.l;
    t = other.t;
    r = other.r;
    b = other.b;
    return *this;
  }

  inline bool Equal(T l, T t, T r, T b) {
    return this->l == l
        && this->t == t
        && this->r == r
        && this->b == b;
  }

  /**
   * Left + Right
   * @return
   */
  inline T lr() const {
    return left + right;
  }

  /**
   * Top + Bottom
   * @return
   */
  inline T tb() const {
    return top + bottom;
  }

  union { T l, left; };
  union { T t, top; };
  union { T r, right; };
  union { T b, bottom; };
};

template<typename T>
inline bool operator==(const Bounds<T> &s1, const Bounds<T> &s2) {
  return memcmp(&s1, &s2, sizeof(Bounds<T>)) == 0;
}

template<typename T>
inline bool operator!=(const Bounds<T> &s1, const Bounds<T> &s2) {
  return memcmp(&s1, &s2, sizeof(Bounds<T>)) != 0;
}

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_INTERNAL_SIDE_SPACE_HPP_
