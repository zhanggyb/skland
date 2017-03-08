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

#include <skland/gui/video-view.hpp>
#include <skland/gui/surface.hpp>

#include <skland/gui/context.hpp>
#include <skland/graphic/canvas.hpp>
#include <skland/graphic/paint.hpp>

namespace skland {

VideoView::VideoView()
    : VideoView(320, 240) {

}

VideoView::VideoView(int width, int height)
    : AbstractView(width, height),
      surface_(nullptr) {

}

VideoView::~VideoView() {
  delete surface_;
}

void VideoView::OnUpdate(AbstractView *view) {

}

Surface *VideoView::GetSurface(const AbstractView *view) const {
  return surface_;
}

void VideoView::OnSizeChanged(int width, int height) {

}

void VideoView::OnMouseEnter(MouseEvent *event) {

}

void VideoView::OnMouseLeave(MouseEvent *event) {

}

void VideoView::OnMouseMove(MouseEvent *event) {

}

void VideoView::OnMouseButton(MouseEvent *event) {

}

void VideoView::OnKeyboardKey(KeyEvent *event) {

}

void VideoView::OnDraw(const Context *context) {
  std::shared_ptr<Canvas> canvas = context->canvas();
  Paint paint;
  paint.SetColor(0xFF29FF29);
  canvas->DrawRect(GetGeometry(), paint);
}

}
