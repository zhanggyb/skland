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

#ifndef SKLAND_CORE_NUMERIC_HPP_
#define SKLAND_CORE_NUMERIC_HPP_

#include <algorithm>

namespace skland {
namespace core {

/**
 * @ingroup core
 * @brief Helper class for bit operation
 */
class Bit {

 public:

  template<typename T>
  static inline void Set(T &x, T y) {
    x |= y;
  }

  template<typename T>
  static inline void Clear(T &x, T y) {
    x &= ~y;
  }

  template<typename T>
  static inline void Inverse(T &x, T y) {
    x = (x & (~y)) | (x ^ y);
  }

};

/**
 * @ingroup core
 * @brief Clamp given value between min and max
 */
template<typename T>
inline T clamp(T x, T min_value, T max_value) {
  return std::min(std::max(x, min_value), max_value);
}

} // namespace core
} // namespace skland

#endif // SKLAND_CORE_NUMERIC_HPP_
