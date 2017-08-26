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

#include "draw-test.hpp"

#include "skland/core/color.hpp"
#include "skland/core/rect.hpp"
#include "skland/graphic/paint.hpp"
#include "skland/graphic/canvas.hpp"
#include "skland/graphic/bitmap.hpp"

using namespace skland;
using namespace skland::core;
using namespace skland::graphic;

static const int kWidth = 400;
static const int kHeight = 300;
static const std::string kFileNamePrefix("graphic_canvas_");

DrawTest::DrawTest() {}

DrawTest::~DrawTest() {}

TEST_F(DrawTest, draw_line_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  paint.SetColor(0xFFFF0000);
  paint.SetStrokeWidth(2.f);
  paint.SetStyle(Paint::kStyleStroke);
  canvas.DrawLine(50.f, 50.f, 350.f, 250.f, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "draw_line_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}

TEST_F(DrawTest, draw_rect_1) {
  Bitmap bitmap;
  bitmap.AllocateN32Pixels(kWidth, kHeight);

  Canvas canvas(bitmap);
  canvas.Clear(0xFFFFFFFF);

  Paint paint;
  paint.SetColor(0xFFFF0000);
  RectF rect = RectF::MakeFromXYWH(0.f, 0.f, kWidth, kHeight);
  rect = rect.Inset(50.f);
  canvas.DrawRect(rect, paint);
  canvas.Flush();

  std::string filename = kFileNamePrefix + "draw_rect_1.png";
  bitmap.WriteToFile(filename);

  std::cout << std::endl
            << "Check image file: " << filename
            << std::endl
            << std::endl;

  ASSERT_TRUE(true);
}
