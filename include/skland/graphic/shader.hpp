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

#ifndef SKLAND_GRAPHIC_SHADER_HPP_
#define SKLAND_GRAPHIC_SHADER_HPP_

#include <memory>

namespace skland {
namespace graphic {

class Paint;
class GradientShader;

/**
 * @ingroup graphic
 * @brief The base shader class
 */
class Shader {

  friend class Paint;
  friend class GradientShader;

 public:

  enum TileMode {
    kTileModeClamp,
    kTileModeRepeat,
    kTileModeMirror
  };

  Shader();

  Shader(const Shader &other);

  ~Shader();

  Shader &operator=(const Shader &other);

  explicit operator bool() const;

 protected:

  struct Private;

  explicit Shader(Private *p);

 private:

  std::unique_ptr<Private> p_;

};

} // namespace graphic
} // namespace skland

#endif  // SKLAND_GRAPHIC_SHADER_HPP_
