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
#include "skland/gui/abstract-gl-interface.hpp"
#include "skland/gui/region.hpp"

namespace skland {
namespace gui {

GLView::GLView() {
  callback_.done().Set(this, &GLView::OnFrame);
}

GLView::~GLView() {
  // Note: delete interface_ before destroying gl_surface_:
  delete interface_;
  delete gl_surface_;
}

void GLView::SetGLInterface(AbstractGLInterface *interface) {
  if (interface_ == interface)
    return;

  interface_ = interface;

  if (nullptr != gl_surface_) {
    gl_surface_->SetGLInterface(interface_);
  }
}

void GLView::OnRequestUpdate(AbstractView *view) {
  if ((view == this) && (nullptr != gl_surface_)) {
    gl_surface_->Update();
    return;
  }

  AbstractView::OnRequestUpdate(view);
}

void GLView::OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  RequestSaveGeometry(old_geometry != new_geometry);

  if (nullptr != gl_surface_) {
//    if ((old_geometry.width() != new_geometry.width()) || (old_geometry.height() != new_geometry.height())) {
      interface_->SetViewportSize((int) new_geometry.width(), (int) new_geometry.height());
      OnResize((int) new_geometry.width(), (int) new_geometry.height());
//    }
  }
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
    gl_surface_->SetGLInterface(interface_);
//    gl_surface_->SetCommitMode(Surface::kDesynchronized);

    Region region;
    gl_surface_->SetInputRegion(region);

    interface_->SetViewportSize(GetWidth(), GetHeight());
    callback_.Setup(*gl_surface_);
    OnInitialize();
  }

  _ASSERT(nullptr != gl_surface_);
  _ASSERT(gl_surface_->GetParent() == context.surface());
  Surface::Sub::Get(gl_surface_)->SetWindowPosition(GetX(), GetY());
  gl_surface_->Commit();
}

void GLView::OnRenderSurface(Surface *surface) {
  _ASSERT(gl_surface_ == surface);
  Surface::Sub::Get(gl_surface_)->SetWindowPosition(GetX(), GetY());
  gl_surface_->Commit();
}

void GLView::SwapBuffers() {
  if (nullptr != interface_) interface_->SwapBuffers();
}

void GLView::MakeCurrent() {
  if (nullptr != interface_) interface_->MakeCurrent();
}

void GLView::OnFrame(uint32_t serial) {
  callback_.Setup(*gl_surface_);
  OnRender();
  gl_surface_->Commit();
}

} // namespace gui
} // namespace skland
