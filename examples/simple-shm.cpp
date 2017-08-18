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
#include <skland/gui/abstract-view.hpp>
#include <skland/gui/context.hpp>
#include <skland/gui/callback.hpp>
#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include "skland/numerical/clamp.hpp"

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <iostream>

using namespace skland;
using namespace skland::core;
using namespace skland::gui;
using namespace skland::graphic;
using namespace skland::numerical;

class SpinningView : public AbstractView {

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(SpinningView);

  SpinningView() {
    color_ = 0xFF4FBF4F;
    frame_callback_.done().Set(this, &SpinningView::OnFrame);
  }

  ~SpinningView() override = default;

 protected:

  void OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) override {
    if (!running_animation_)
      RequestSaveGeometry(old_geometry != new_geometry);
  }

  void OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) override {
    Update(old_geometry != new_geometry);
  }

  void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  void OnMouseLeave() override {

  }

  void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  void OnMouseDown(MouseEvent *event) override {
    event->Ignore();
  }

  void OnMouseUp(MouseEvent *event) override {
    event->Ignore();
  }

  void OnKeyDown(KeyEvent *event) override {
    event->Ignore();
  }

  void OnKeyUp(KeyEvent *event) override {
    event->Ignore();
  }

  void OnDraw(const Context &context) override {
    if (!running_animation_) running_animation_ = true;

    angle_ += 5.f;
    if (angle_ > 360.f) angle_ = 0.f;

    frame_callback_.Setup(*context.surface());
    Animate(context);
  }

 private:

  void Animate(const Context &context) {
    static const int padding = 0;
    Canvas *canvas = context.canvas();
    canvas->Save();
    int scale = context.surface()->GetScale();
    canvas->Scale(scale, scale);

    const RectF &rect = GetGeometry();
    float radius_ = Clamp(std::min(rect.width(), rect.height()) / 2.f - 50.f,
                          50.f, 200.f);

    Paint paint;
    paint.SetAntiAlias(true);

    paint.SetColor(color_);
    paint.SetStyle(Paint::Style::kStyleFill);
    canvas->DrawRect(RectF(rect.l + padding,
                           rect.t,
                           rect.r - padding,
                           rect.b - padding),
                     paint);

    paint.SetColor(ColorF(0xEFFFFFFF));
    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetStrokeWidth(5.f);

    canvas->DrawArc(RectF(rect.center_x() - radius_,
                          rect.center_y() - radius_,
                          rect.center_x() + radius_,
                          rect.center_y() + radius_),
                    angle_, 300.f, false, paint);
    canvas->Restore();
  }

  void OnFrame(uint32_t serial) {
    Update();
  }

  Callback frame_callback_;
  ColorF color_;
  float angle_ = 0.f;
  bool running_animation_ = false;
};

int main(int argc, char *argv[]) {
  Application app(argc, argv);

  auto *win = new Window(320, 280, "Simple Shm");
  win->SetAppId("Simple-Shm");

  auto *widget = new SpinningView;
  win->SetContentView(widget);

  win->Show();

  return app.Run();
}
