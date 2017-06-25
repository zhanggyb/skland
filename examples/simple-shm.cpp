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

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>
#include <iostream>
#include <skland/gui/callback.hpp>

using namespace skland;
using namespace skland::core;
using namespace skland::gui;
using namespace skland::graphic;

class SpinningView : public AbstractView {

 public:

  SpinningView()
      : AbstractView(),
        context_(nullptr),
        angle_(0.f),
        running_animation_(false) {
    color_ = 0xFF4FBF4F;
    frame_callback_.done().Set(this, &SpinningView::OnFrame);
  }

  virtual ~SpinningView() {}

 protected:

  virtual void OnConfigureGeometry(int dirty_flag, const RectF &old_geometry, const RectF &new_geometry) override {
    if (!running_animation_)
      Update();
  }

  virtual void OnGeometryChange(int dirty_flag, const RectF &old_geometry, const RectF &new_geometry) override {

  }

  virtual void OnLayout(int dirty_flag, int left, int top, int right, int bottom) final {

  }

  virtual void OnMouseEnter(MouseEvent *event) override {
    event->Accept();
  }

  virtual void OnMouseLeave() override {

  }

  virtual void OnMouseMove(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseDown(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnMouseUp(MouseEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyDown(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnKeyUp(KeyEvent *event) override {
    event->Ignore();
  }

  virtual void OnDraw(const Context *context) override {
    context_ = context;

    OnFrame(0);
  }

 private:

  void Animate() {
    static const int padding = 0;
    Canvas *canvas = context_->canvas();
    canvas->Save();
    int scale = context_->surface()->GetScale();
    canvas->Scale(scale, scale);

    const RectF &rect = GetGeometry();
    float radius_ = clamp(std::min(rect.width(), rect.height()) / 2.f - 50.f,
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
    if (!running_animation_) running_animation_ = true;

    angle_ += 5.f;
    if (angle_ > 360.f) angle_ = 0.f;

    frame_callback_.Setup(*context_->surface());
    Animate();
    context_->surface()->Damage(context_->surface()->GetMargin().l + GetX(),
                                context_->surface()->GetMargin().t + GetY(),
                                GetWidth(),
                                GetHeight());
    context_->surface()->Commit();
  }

  Callback frame_callback_;
  const Context *context_;
  ColorF color_;
  float angle_;
  bool running_animation_;
};

int main(int argc, char *argv[]) {
  Application app(argc, argv);

  Window *win = new Window(320, 280, "Simple Shm");
  win->SetAppId("Simple-Shm");

  SpinningView *widget = new SpinningView;
  win->SetContentView(widget);

  win->Show();

  return app.Run();
}
