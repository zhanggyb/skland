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

#include "skland/gui/gl-view.hpp"

#include "skland/core/assert.hpp"

#include "skland/gui/surface.hpp"
#include "skland/gui/mouse-event.hpp"
#include "skland/gui/key-event.hpp"
#include "skland/gui/context.hpp"

namespace skland {
namespace gui {

GLView::GLView() {
  callback_.done().Set(this, &GLView::OnFrame);
}

GLView::~GLView() {
  delete gl_surface_;
}

void GLView::OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  RequestSaveGeometry(old_geometry != new_geometry);
}

void GLView::OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  Update();
}

void GLView::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void GLView::OnMouseLeave() {

}

void GLView::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void GLView::OnMouseDown(MouseEvent *event) {
  event->Accept();
}

void GLView::OnMouseUp(MouseEvent *event) {
  event->Accept();
}

void GLView::OnKeyDown(KeyEvent *event) {
  event->Accept();
}

void GLView::OnKeyUp(KeyEvent *event) {
  event->Accept();
}

void GLView::OnDraw(const Context &context) {
  if (nullptr == gl_surface_) {
    gl_surface_ = Surface::Sub::Create(context.surface(), this);
  }
}

void GLView::OnRenderSurface(Surface *surface) {

}

void GLView::OnFrame(uint32_t serial) {

}

} // namespace gui
} // namespace skland
