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

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>
#include <skland/gui/abstract-widget.hpp>

#include <skland/core/color.hpp>
#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include <iostream>
#include <skland/stock/theme.hpp>

#include "SkTypeface.h"
#include "SkPaint.h"
#include "SkCanvas.h"

#include <skland/graphic/gradient-shader.hpp>

using namespace skland;

class SimpleWidget : public AbstractWidget {

  sk_sp<SkTypeface> font_;
  Shader linear_shader_;
  Shader radial_shader_;

 public:

  SimpleWidget()
      : AbstractWidget() {
    set_name("Simple Widget");
//    font_ = SkTypeface::MakeFromFile("SourceHanSansCN-Regular.otf");
    font_ = SkTypeface::MakeFromName("Arial", SkFontStyle(400, 5, SkFontStyle::kUpright_Slant));

    Point2F points[2];
    points[0].x = 100.f;
    points[0].y = 100.f;
    points[1].x = 300.f;
    points[1].y = 300.f;

    Color colors[2];
    colors[0] = 0xFFF3F80A;
    colors[1] = 0xFFF52597;

    float pos[2] = {0.f, 1.f};

    linear_shader_ =
        GradientShader::MakeLinear(points, colors, pos, 2, Shader::kTileModeClamp);

    Point2F center = {350.f, 350.f};
    Color colors2[2];
    colors2[0] = 0xFF9F9F9F;
    colors2[1] = 0xFF444444;
    float pos2[2] = {0.33f, 1.f};

    radial_shader_ =
        GradientShader::MakeRadial(center, 50.f, colors2, pos2, 2, Shader::kTileModeClamp);
  }

  virtual ~SimpleWidget() {
  }

 protected:

  virtual void OnResize(int width, int height) override {
    resize(width, height);
    Show();
  }

  virtual void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseLeave(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseButton(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyboardKey(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnDraw(Canvas *canvas) override {
    Paint paint;
//    paint.SetColor(Color(0.055f, 0.125f, 0.165f, 1.f));
//    canvas->DrawRectangle(x(), y(), width(), height(), paint);

    Paint p2;
    p2.SetShader(linear_shader_);
    canvas->DrawRect(Rect(100, 100, 200, 200), p2);

    Paint p3;
    p3.SetAntiAlias(true);
    p3.SetShader(radial_shader_);
    canvas->DrawCircle(350, 350, 50, p3);

    SkPaint paint1;

    paint1.setTextSize(64.0f);
    paint1.setAntiAlias(true);
    paint1.setColor(0xff4281A4);
    paint1.setStyle(SkPaint::kFill_Style);
    paint1.setTypeface(font_);

    const char text[] = "Hello World!";
    canvas->sk_canvas()->drawText(text, strlen(text), 220.0f, 320.0f, paint1);
  }

};

int main(int argc, char *argv[]) {

  Application app(argc, argv);

  Window *win = new Window(800, 600, "Simple");

  SimpleWidget *widget = new SimpleWidget;
  win->SetMainWidget(widget);

  win->Show();

  return app.Run();
}
