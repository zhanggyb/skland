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
      : AbstractWidget(), context_(nullptr), radius_(0.f), angle_(0.f) {
    color_ = 0xFF4FBF4F;
    callback_.done().Set(this, &ShmWidget::OnFrame);

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
    static int padding = 5;
    context_ = context;
    std::shared_ptr<Canvas> canvas = context->GetCanvas();
    canvas->Save();
    canvas->ClipRect(Rect(geometry().l + padding,
                          geometry().t + padding,
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

    context_->SetupCallback(callback_);
    context_->Damage(context_->GetMargin().l + (int) geometry().l,
                     context_->GetMargin().t + (int) geometry().t,
                     (int) geometry().width(),
                     (int) geometry().height());
    context_->Commit();
  }

 private:

  void OnFrame(uint32_t serial) {

    angle_ += 5.f;
    if (angle_ > 360.f) angle_ = 0.f;

    OnDraw(context_);
  }

  wayland::Callback callback_;
  const Context *context_;
  Color color_;
  float radius_;
  float angle_;
};

int main(int argc, char *argv[]) {
  using skland::Window;

  Application app(argc, argv);

  Window *win = new Window(320, 320, "Simple Shm");
  win->SetAppId("Simple-Shm");

  ShmWidget *widget = new ShmWidget;
  win->SetMainWidget(widget);

  win->Show();

  return app.Run();
}
