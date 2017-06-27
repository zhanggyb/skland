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

#include <skland/gui/push-button.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/context.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>
#include <skland/graphic/text-box.hpp>
#include <skland/graphic/path.hpp>
#include <skland/graphic/gradient-shader.hpp>

#include <skland/gui/theme.hpp>

namespace skland {
namespace gui {

using core::RectF;
using graphic::Canvas;
using graphic::Paint;
using graphic::Path;
using graphic::Shader;
using graphic::Font;
using graphic::TextBox;

PushButton::PushButton(const std::string &text)
    : AbstractButton(text) {

}

PushButton::~PushButton() {

}

void PushButton::OnDraw(const Context *context) {
  Canvas *canvas = context->canvas();

  const RectF &geometry = GetGeometry();
  int scale = context->surface()->GetScale();
  Canvas::ClipGuard guard(canvas, geometry * scale);

  canvas->Clear();
  canvas->Scale(scale, scale);

  Path path;
  RectF inner_rect = geometry.Shrink(0.5f);
  const Theme::Schema &schema = Theme::GetData().button;
  Shader shader;
  core::Point2F points[2];
  points[0].x = geometry.left;
  points[0].y = geometry.top;
  points[1].x = geometry.left;
  points[1].y = geometry.bottom;

  float radii[] = {
      4.f, 4.f,
      4.f, 4.f,
      4.f, 4.f,
      4.f, 4.f
  };

  path.AddRoundRect(inner_rect, radii);

  Paint paint;
  paint.SetAntiAlias(true);
  if (IsHovered()) {
    if (IsPressed()) {
      if (schema.active.background.shaded) {
        shader = Theme::Helper::GradientShader::MakeLinear(points, schema.active.background);
        paint.SetShader(shader);
      } else {
        paint.SetColor(Theme::GetData().button.active.background.color);
      }
    } else {
      if (schema.highlight.background.shaded) {
        shader = Theme::Helper::GradientShader::MakeLinear(points, schema.highlight.background);
        paint.SetShader(shader);
      } else {
        paint.SetColor(Theme::GetData().button.highlight.background.color);
      }
    }
  } else {
    if (schema.inactive.background.shaded) {
      shader = Theme::Helper::GradientShader::MakeLinear(points, schema.inactive.background);
      paint.SetShader(shader);
    } else {
      paint.SetColor(Theme::GetData().button.inactive.background.color);
    }
  }

  canvas->DrawPath(path, paint);
  paint.SetShader(Shader());

  paint.SetStyle(Paint::Style::kStyleStroke);
  paint.SetColor(schema.inactive.outline.color);
  path.Reset();
  path.AddRoundRect(inner_rect, radii);
  canvas->DrawPath(path, paint);

  const Font &font = GetFont();
  const std::string &text = GetText();

  paint.SetColor(schema.inactive.foreground.color);
  paint.SetStyle(Paint::kStyleFill);
  paint.SetFont(font);
  paint.SetTextSize(font.GetSize());

  float text_width = paint.MeasureText(text.c_str(), text.length());

  TextBox text_box;
  // Put the text at the center
  text_box.SetBox(geometry.l + (geometry.width() - text_width) / 2.f,
                  geometry.t + 1.f, // move down a little for better look
                  geometry.r - (geometry.width() - text_width) / 2.f,
                  geometry.b);
  text_box.SetSpacingAlign(TextBox::kSpacingAlignCenter);
  text_box.SetText(text.c_str(), text.length(), paint);
  text_box.Draw(*canvas);
}

} // namespace gui
} // namespace skland
