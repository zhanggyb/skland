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

#include <skland/gui/shell-surface.hpp>
#include <skland/gui/surface.hpp>

#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/popup-shell-surface.hpp>

#include "internal/display-proxy.hpp"

namespace skland {

Surface *ShellSurface::Create(AbstractView *view, const Margin &margin) {
  Surface *surface = new Surface(view, margin);
  surface->role_.shell_surface = new ShellSurface(surface);
  return surface;
}

ShellSurface *ShellSurface::Get(const Surface *surface) {
  if (nullptr == surface->parent_)
    return surface->role_.shell_surface;

  return nullptr;
}

ShellSurface::ShellSurface(Surface *surface)
    : surface_(surface), parent_(nullptr) {
  DBG_ASSERT(surface_);
  role_.placeholder = nullptr;
  xdg_surface_.Setup(DisplayProxy().xdg_shell(), surface_->wl_surface_);

  PushShellSurface();
}

ShellSurface::~ShellSurface() {
  RemoveShellSurface();

  if (nullptr == parent_) delete role_.toplevel_shell_surface;
  else delete role_.popup_shell_surface_;

  xdg_surface_.Destroy();

  DBG_ASSERT(surface_->role_.shell_surface == this);
  surface_->role_.shell_surface = nullptr;
}

void ShellSurface::ResizeWindow(int width, int height) const {
  xdg_surface_.SetWindowGeometry(surface()->margin().l,
                                 surface()->margin().t,
                                 width, height);
}

void ShellSurface::PushShellSurface() {
  DBG_ASSERT(nullptr == surface_->parent_);
  DBG_ASSERT(nullptr == surface_->up_);
  DBG_ASSERT(nullptr == surface_->down_);

  DBG_ASSERT(Surface::kShellSurfaceCount >= 0);

  if (Surface::kTop) {
    Surface::kTop->up_ = surface_;
    surface_->down_ = Surface::kTop;
    Surface::kTop = surface_;
  } else {
    DBG_ASSERT(Surface::kShellSurfaceCount == 0);
    DBG_ASSERT(nullptr == Surface::kBottom);
    Surface::kBottom = surface_;
    Surface::kTop = surface_;
  }

  Surface::kShellSurfaceCount++;
}

void ShellSurface::RemoveShellSurface() {
  DBG_ASSERT(nullptr == surface_->parent_);

  if (surface_->up_) {
    surface_->up_->down_ = surface_->down_;
  } else {
    DBG_ASSERT(Surface::kTop == surface_);
    Surface::kTop = surface_->down_;
  }

  if (surface_->down_) {
    surface_->down_->up_ = surface_->up_;
  } else {
    DBG_ASSERT(Surface::kBottom == surface_);
    Surface::kBottom = surface_->up_;
  }

  surface_->up_ = nullptr;
  surface_->down_ = nullptr;
  Surface::kShellSurfaceCount--;
  DBG_ASSERT(Surface::kShellSurfaceCount >= 0);
}

}
