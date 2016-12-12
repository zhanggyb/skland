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
#include <skia.h>

#include "SkCanvas.h"
#include "SkPaint.h"

using namespace skland;

class SimpleWidget : public AbstractWidget {

 public:

  SimpleWidget()
      : AbstractWidget() {
    set_name("Simple Widget");
  }

  virtual ~SimpleWidget() {}

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
    SkCanvas *c = canvas->sk_canvas();
    Paint paint;
    paint.SetColor(Color(1.f, 1.f, 1.f));

    canvas->DrawRectangle(x(), y(), width(), height(), paint);

    /*
    std::vector<uint32_t> pixels(250 * 250, 0);
    SkImageInfo image_info = SkImageInfo::MakeN32Premul(250, 250);
    SkPixmap pixmap(image_info,
                    pixels.data(),
                    250 * 4);

    std::unique_ptr<SkCanvas> canvas2 =
        SkCanvas::MakeRasterDirectN32(250, 250, pixels.data(), 250 * 4);

    SkPaint paint2;
    paint2.setAntiAlias(true);
    paint2.setARGB(255, 0, 0, 0);
    paint2.setMaskFilter(SkBlurMaskFilter::Make(
        kNormal_SkBlurStyle, 35.f, 0x2));

    canvas2->drawRect(SkRect::MakeLTRB(35,
                                      35,
                                      250 - 35,
                                      250 - 35),
                     paint2);

    sk_sp<SkImage> image = SkImage::MakeFromRaster(pixmap, nullptr, nullptr);
    c->drawImageRect(image, SkRect::MakeLTRB(0, 0, 250, 250),
                     SkRect::MakeLTRB(100, 100, 350, 350),
                     &paint2);
    */

    SkPaint paint2;
    paint2.setAntiAlias(true);
    paint2.setStyle(SkPaint::kFill_Style);

    sk_sp<SkImage> image = SkImage::MakeFromRaster(*Theme::shadow_pixmap(), nullptr, nullptr);
    c->drawImageRect(image, SkRect::MakeLTRB(0, 0, 250, 250),
                     SkRect::MakeLTRB(100, 100, 350, 350),
                     nullptr);
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
