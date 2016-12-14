//
// Created by zhanggyb on 16-12-14.
//

#include <skland/graphic/gradient-shader.hpp>
#include <skland/graphic/matrix.hpp>

#include "internal/meta-matrix.hpp"
#include "internal/meta-shader.hpp"

#include "SkGradientShader.h"

namespace skland {

Shader GradientShader::MakeLinear(const Point2F *points,
                                  const uint32_t *colors,
                                  const float *pos,
                                  int count,
                                  Shader::TileMode mode,
                                  uint32_t flags,
                                  const Matrix *local_matrix) {
  sk_sp<SkShader> sk_shader;

  if (nullptr == local_matrix) {
    sk_shader =
        SkGradientShader::MakeLinear(reinterpret_cast<const SkPoint *>(points),
                                     colors,
                                     pos,
                                     count,
                                     (SkShader::TileMode) mode,
                                     flags, nullptr);
  } else {
    sk_shader =
        SkGradientShader::MakeLinear(reinterpret_cast<const SkPoint *>(points),
                                     colors,
                                     pos,
                                     count,
                                     (SkShader::TileMode) mode,
                                     flags,
                                     &local_matrix->metadata()->sk_matrix);
  }

  return Shader(new graphic::MetaShader(sk_shader));
}

Shader GradientShader::MakeLinear(const Point2F points[],
                                  const Color colors[],
                                  const float pos[],
                                  int count,
                                  Shader::TileMode mode,
                                  uint32_t flags,
                                  const Matrix *local_matrix) {
  sk_sp<SkShader> sk_shader;

  if (nullptr == local_matrix) {
    sk_shader =
        SkGradientShader::MakeLinear(reinterpret_cast<const SkPoint *>(points),
                                     reinterpret_cast<const SkColor4f *>(colors),
                                     nullptr, // TODO: use ColorSpace
                                     pos,
                                     count,
                                     (SkShader::TileMode) mode,
                                     flags,
                                     nullptr);
  } else {
    sk_shader =
        SkGradientShader::MakeLinear(reinterpret_cast<const SkPoint *>(points),
                                     reinterpret_cast<const SkColor4f *>(colors),
                                     nullptr, // TODO: use ColorSpace
                                     pos,
                                     count,
                                     (SkShader::TileMode) mode,
                                     flags,
                                     &local_matrix->metadata()->sk_matrix);
  }

  return Shader(new graphic::MetaShader(sk_shader));
}

}
