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
#include <skland/gui/display.hpp>

namespace skland {

ShellSurface::ShellSurface(AbstractView *view, const Margin &margin)
    : Trackable(), surface_holder_(view, margin) {
  surface_holder_.surface_destroying().Connect(this, &ShellSurface::OnViewSurfaceDestroying);
  xdg_surface_.Setup(Display::xdg_shell(), surface_holder_.wl_surface());
  surface_holder_.PushShellSurface();
}

ShellSurface::~ShellSurface() {
  UnbindAll();
  surface_holder_.RemoveShellSurface();
  xdg_surface_.Destroy();
}

void ShellSurface::OnViewSurfaceDestroying(SLOT) {
  destroying_.Emit();
  xdg_surface_.Destroy();
}

}
