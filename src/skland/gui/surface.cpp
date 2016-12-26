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

#include <skland/gui/surface.hpp>
#include <skland/gui/display.hpp>
#include <skland/gui/main-window.hpp>
#include <skland/gui/buffer.hpp>
#include <skland/gui/output.hpp>

namespace skland {

Surface::Surface(AbstractView *view, const Margin &margin)
    : Object(),
      view_(view),
      buffer_transform_(WL_OUTPUT_TRANSFORM_NORMAL),
      buffer_scale_(1),
      margin_(margin) {
  DBG_ASSERT(view_ != nullptr);
  wl_surface_.enter().Set(this, &Surface::OnEnter);
  wl_surface_.leave().Set(this, &Surface::OnLeave);
  wl_surface_.Setup(Display::wl_compositor());
  wl_surface_.SetUserData(this);
}

Surface::~Surface() {

  // clear all sub surfaces
  ClearChildren();

  if (view_) {
    // TODO: remove from role
  }
}

void Surface::AddSubSurface(Surface *subsurface, int pos) {
  if (subsurface == nullptr || subsurface->parent() == this) return;

  InsertChild(subsurface, pos);
}

void Surface::Attach(const Buffer &buffer, int32_t x, int32_t y) {
  if (buffer.wl_buffer().IsValid()) {
    canvas_sp_.reset(new Canvas((unsigned char *) buffer.pixel(),
                                buffer.size().width,
                                buffer.size().height));
    canvas_sp_->SetOrigin((float) margin_.left, (float) margin_.top);
    wl_surface_.Attach(buffer.wl_buffer(), x, y);
  } else {
    canvas_sp_.reset();
    wl_surface_.Attach(NULL, x, y);
  }
}

void Surface::SetInputRegion(const wayland::Region &region) {
  wl_surface_.SetInputRegion(region);
}

void Surface::SetOpaqueRegion(const wayland::Region &region) {
  wl_surface_.SetOpaqueRegion(region);
}

Surface *Surface::GetSubSurfaceAt(int index) const {
  return static_cast<Surface *>(GetChildAt(index));
}

Surface *Surface::RemoveSubSurface(Surface *subsurface) {
  if (RemoveChild(subsurface)) {
    return subsurface;
  }

  return nullptr;
}

void Surface::Commit() const {
  wl_surface_.Commit();
  if (parent()) {
    static_cast<Surface *>(parent())->Commit();
  }
}

void Surface::OnEnter(struct wl_output *wl_output) {
}

void Surface::OnLeave(struct wl_output *wl_output) {
}

}
