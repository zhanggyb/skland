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
#include "skland/gui/abstract-rendering-api.hpp"
#include "skland/gui/region.hpp"

namespace skland {
namespace gui {

GLView::GLView() {
  callback_.done().Set(this, &GLView::OnFrame);
}

GLView::~GLView() {
  // Note: delete interface_ before destroying gl_surface_:
  delete rendering_api_;
  delete gl_surface_;
}

void GLView::SetRenderingInterface(AbstractRenderingAPI *api) {
  if (rendering_api_ == api) return;

  rendering_api_ = api;

  if (nullptr != gl_surface_) {
    gl_surface_->SetRenderingAPI(rendering_api_);
  }
}

void GLView::OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) {
  if (!RequestSaveGeometry(new_geometry)) return;

  if ((nullptr != gl_surface_) && (nullptr != rendering_api_)) {
    rendering_api_->SetViewportSize((int) new_geometry.width(), (int) new_geometry.height());
    OnResize((int) new_geometry.width(), (int) new_geometry.height());
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
    if (nullptr == rendering_api_) return;

    gl_surface_ = Surface::Sub::Create(context.surface(), this);
    gl_surface_->SetRenderingAPI(rendering_api_);
//    gl_surface_->SetCommitMode(Surface::kDesynchronized);

    Region region;
    gl_surface_->SetInputRegion(region);

    rendering_api_->SetViewportSize(GetWidth(), GetHeight());
    Surface::Sub::Get(gl_surface_)->SetWindowPosition(GetX(), GetY());

    callback_.Setup(*gl_surface_);
    OnInitialize();
    gl_surface_->Commit();
  }
}

void GLView::OnRenderSurface(Surface *surface) {

}

void GLView::SwapBuffers() {
  if (nullptr != rendering_api_) rendering_api_->SwapBuffers();
}

void GLView::MakeCurrent() {
  if (nullptr != rendering_api_) rendering_api_->MakeCurrent();
}

void GLView::OnFrame(uint32_t serial) {
  callback_.Setup(*gl_surface_);
  OnRender();
  gl_surface_->Commit();
}

} // namespace gui
} // namespace skland
