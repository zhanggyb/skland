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

#include "test.hpp"

#include "skland/graphic/gradient-shader.hpp"
#include "skland/graphic/bitmap.hpp"
#include "skland/graphic/paint.hpp"
#include "skland/graphic/canvas.hpp"

#include "SkGradientShader.h"

using namespace skland;
using namespace skland::core;
using namespace skland::graphic;

static const int kWidth = 400;
static const int kHeight = 300;
static const std::string kFileNamePrefix("graphic_gradient_shader_");

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, linear_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;

  PointF points[2] = {{0.f, 0.f}, {0.f, 300.f}};
  uint32_t colors[2] = {0xFFFFFFFF, 0xFF000000};
  float pos[2] = {0.f, 1.f};

  Shader shader = GradientShader::MakeLinear(points, colors, pos, 2, Shader::kTileModeClamp);
  paint.SetShader(shader);

  RectF rect = RectF::MakeFromXYWH(0.f, 0.f, kWidth, kHeight);

  canvas.DrawRect(rect, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "linear_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(Test, linear_2) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;

  PointF points[2] = {{0.f, 0.f}, {0.f, kHeight}};
  ColorF colors[2];
  colors[0].red = 1.f;
  colors[0].green = 1.f;
  colors[0].blue = 1.f;
  colors[1].red = 0.f;
  colors[1].green = 0.f;
  colors[1].blue = 0.f;

  float pos[2] = {0.f, 1.f};

  Shader shader = GradientShader::MakeLinear(points, colors, pos, 2, Shader::kTileModeClamp);
  paint.SetShader(shader);

  RectF rect = RectF::MakeFromXYWH(0.f, 0.f, kWidth, kHeight);

  canvas.DrawRect(rect, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "linear_2.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(Test, radial_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;

  PointF center(kWidth / 2.f, kHeight / 2.f);
  float radius = std::min(kWidth, kHeight) / 2.f;
  uint32_t colors[2] = {0xFFFFFFFF, 0xFF000000};
  float pos[2] = {0.f, 1.f};

  Shader shader = GradientShader::MakeRadial(center, radius, colors, pos, 2, Shader::kTileModeClamp);
  paint.SetShader(shader);

  RectF rect = RectF::MakeFromXYWH(0.f, 0.f, kWidth, kHeight);

  canvas.DrawRect(rect, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "radial_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}


TEST_F(Test, radial_2) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;

  PointF center(kWidth / 2.f, kHeight / 2.f);
  float radius = std::min(kWidth, kHeight) / 2.f;
  ColorF colors[2];
  colors[0].red = 1.f;
  colors[0].green = 1.f;
  colors[0].blue = 1.f;
  colors[1].red = 0.f;
  colors[1].green = 0.f;
  colors[1].blue = 0.f;
  float pos[2] = {0.f, 1.f};

  Shader shader = GradientShader::MakeRadial(center, radius, colors, pos, 2, Shader::kTileModeClamp);
  paint.SetShader(shader);

  RectF rect = RectF::MakeFromXYWH(0.f, 0.f, kWidth, kHeight);

  canvas.DrawRect(rect, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "radial_2.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}