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
#include <skland/gui/context.hpp>

#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>
#include <iostream>

using namespace skland;

class ShmWidget : public AbstractWidget {

 public:

  ShmWidget()
      : AbstractWidget(), context_(nullptr), radius_(0.f), angle_(0.f), running_animation_(false) {
    color_ = 0xFF4FBF4F;
    frame_callback_.done().Set(this, &ShmWidget::OnFrame);

    radius_ = clamp(std::min(geometry().width(), geometry().height()) / 2.f - 50.f, 50.f, 200.f);
  }

  virtual ~ShmWidget() {}

 protected:

  virtual void OnResize(int width, int height) override {
    resize(width, height);
    radius_ = clamp(std::min(geometry().width(), geometry().height()) / 2.f - 50.f, 50.f, 200.f);
    Update();
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

  virtual void OnDraw(const Context *context) override {
    context_ = context;

    if (!running_animation_) {
      running_animation_ = true;
      OnFrame(0);
    }
  }

 private:

  void Animate() {
    static int padding = 5;
    std::shared_ptr<Canvas> canvas = context_->canvas();
    canvas->Save();
    canvas->ClipRect(Rect(geometry().l + padding,
                          geometry().t,
                          geometry().r - padding,
                          geometry().b - padding));

    canvas->Clear(color_);

    Paint paint;
    paint.SetAntiAlias(true);
    paint.SetColor(Color(0xEFFFFFFF));
    paint.SetStyle(Paint::Style::kStyleStroke);
    paint.SetStrokeWidth(5.f);

    canvas->DrawArc(Rect(geometry().center_x() - radius_,
                         geometry().center_y() - radius_,
                         geometry().center_x() + radius_,
                         geometry().center_y() + radius_),
                    angle_, 300.f, false, paint);

    canvas->Restore();
  }

  void OnFrame(uint32_t serial) {
    angle_ += 5.f;
    if (angle_ > 360.f) angle_ = 0.f;

    context_->SetupCallback(frame_callback_);
    Animate();
    context_->Damage(context_->GetMargin().l + x(),
                     context_->GetMargin().t + y(),
                     width(),
                     height());
    context_->Commit();
  }

  wayland::Callback frame_callback_;
  const Context *context_;
  Color color_;
  float radius_;
  float angle_;
  bool running_animation_;
};

int main(int argc, char *argv[]) {
  using skland::Window;

  Application app(argc, argv);

  Window *win = new Window(320, 280, "Simple Shm");
  win->SetAppId("Simple-Shm");

  ShmWidget *widget = new ShmWidget;
  win->SetMainWidget(widget);

  win->Show();

  return app.Run();
}
