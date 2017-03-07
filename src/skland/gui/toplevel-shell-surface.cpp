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

#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/shell-surface.hpp>
#include <skland/core/defines.hpp>

namespace skland {

Surface *ToplevelShellSurface::Create(AbstractEventHandler *event_handler, const Margin &margin) {
  Surface *surface = ShellSurface::Create(event_handler, margin);
  ShellSurface *shell_surface = ShellSurface::Get(surface);
  shell_surface->role_.toplevel_shell_surface = new ToplevelShellSurface(shell_surface);
  return surface;
}

ToplevelShellSurface *ToplevelShellSurface::Get(const Surface *surface) {
  ShellSurface *shell_surface = ShellSurface::Get(surface);

  if (nullptr == shell_surface) return nullptr;

  if (shell_surface->parent_)
    return nullptr;

  return shell_surface->role_.toplevel_shell_surface;
}

ToplevelShellSurface::ToplevelShellSurface(ShellSurface *shell_surface)
    : shell_surface_(shell_surface) {
  xdg_toplevel_.Setup(shell_surface_->xdg_surface_);
}

ToplevelShellSurface::~ToplevelShellSurface() {
  xdg_toplevel_.Destroy();

  DBG_ASSERT(shell_surface_->role_.toplevel_shell_surface == this);
  DBG_ASSERT(nullptr == shell_surface_->parent_);
  shell_surface_->role_.toplevel_shell_surface = nullptr;
}

}
