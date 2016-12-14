//
// Created by zhanggyb on 16-12-14.
//

#ifndef SKLAND_GRADIENT_SHADER_HPP
#define SKLAND_GRADIENT_SHADER_HPP

#include "../core/point.hpp"
#include "../core/color.hpp"

#include "shader.hpp"

namespace skland {

class Matrix;

class GradientShader {

 public:

  static Shader MakeLinear(const Point2F points[2],
                           const uint32_t colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode) {
    return MakeLinear(points, colors, pos, count, mode, 0, nullptr);
  }

  static Shader MakeLinear(const Point2F points[2],
                           const uint32_t colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode,
                           uint32_t flags,
                           const Matrix *local_matrix);

  static Shader MakeLinear(const Point2F points[2],
                           const Color colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode,
                           uint32_t flags,
                           const Matrix *local_matrix);

  static Shader MakeLinear(const Point2F points[2],
                           const Color colors[],
                           const float pos[],
                           int count,
                           Shader::TileMode mode) {
    return MakeLinear(points, colors, pos, count, mode, 0, nullptr);
  }

};

}

#endif //SKLAND_GRADIENT_SHADER_HPP
