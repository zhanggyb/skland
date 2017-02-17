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

#include <skland/gui/sub-surface.hpp>
#include <skland/core/defines.hpp>
#include <skland/gui/display.hpp>

namespace skland {

SubSurface::SubSurface(Surface *parent, AbstractView *view, const Margin &margin)
    : Trackable(), surface_holder_(view, margin) {
  surface_holder_.surface_destroying().Connect(this, &SubSurface::OnSurfaceDestroying);
  wl_sub_surface_.Setup(Display::wl_subcompositor(),
                        surface_holder_.wl_surface(),
                        surface_holder_.wl_surface(parent));

  surface_holder_.SetParent(parent);
}

SubSurface::~SubSurface() {
  UnbindAll();  // Note: Unbind all signals before deleting surface_holder_
  wl_sub_surface_.Destroy();
}

void SubSurface::PlaceAbove(Surface *sibling) {
  if (sibling == surface_holder_.surface()) return;

  if (surface_holder_.surface()->parent() == sibling->parent() ||
      surface_holder_.surface() == sibling->parent() ||
      surface_holder_.surface()->parent() == sibling) {
    wl_sub_surface_.PlaceAbove(surface_holder_.wl_surface(sibling));
    surface_holder_.MoveAbove(sibling);
  }
}

void SubSurface::PlaceBelow(Surface *sibling) {
  if (sibling == surface_holder_.surface()) return;

  if (surface_holder_.surface()->parent() == sibling->parent() ||
      surface_holder_.surface() == sibling->parent() ||
      surface_holder_.surface()->parent() == sibling) {
    wl_sub_surface_.PlaceBelow(surface_holder_.wl_surface(sibling));
    surface_holder_.MoveBelow(sibling);
  }
}

void SubSurface::SetRelativePosition(int x, int y) {
  wl_sub_surface_.SetPosition(x, y);
  surface_holder_.SetRelativePosition(x, y);
}

void SubSurface::SetWindowPosition(int x, int y) {
  Surface* _this_surface = surface_holder_.surface();
  Point parent_global_position = _this_surface->parent()->GetWindowPosition();
  int local_x = x - parent_global_position.x;
  int local_y = y - parent_global_position.y;
  wl_sub_surface_.SetPosition(local_x, local_y);
  surface_holder_.SetRelativePosition(local_x, local_y);
}

void SubSurface::OnSurfaceDestroying(SLOT) {
  wl_sub_surface_.Destroy();
}

}
