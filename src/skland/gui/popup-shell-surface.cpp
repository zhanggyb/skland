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

#include <skland/gui/popup-shell-surface.hpp>
#include <skland/gui/shell-surface.hpp>
#include <skland/core/defines.hpp>

namespace skland {

Surface *PopupShellSurface::Create(ShellSurface *parent, AbstractEventHandler *view, const Margin &margin) {
  Surface *surface = ShellSurface::Create(view, margin);
  ShellSurface *shell_surface = ShellSurface::Get(surface);
  shell_surface->parent_ = parent;
  shell_surface->role_.popup_shell_surface_ = new PopupShellSurface(shell_surface);
  return surface;
}

PopupShellSurface::PopupShellSurface(ShellSurface *shell_surface)
    : shell_surface_(shell_surface) {
  // TODO: initialize xdg_popup_
}

PopupShellSurface::~PopupShellSurface() {
  xdg_popup_.Destroy();

  DBG_ASSERT(shell_surface_->parent_);
  DBG_ASSERT(shell_surface_->role_.popup_shell_surface_ == this);

  shell_surface_->parent_ = nullptr;
  shell_surface_->role_.popup_shell_surface_ = nullptr;
}

}