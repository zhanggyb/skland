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
#include <skland/gui/abstract-surface.hpp>
#include <skland/gui/display.hpp>

namespace skland {

SubSurface::SubSurface(ViewSurface *parent, AbstractView *view, const Margin &margin)
    : Trackable(), surface_holder_(view, margin) {
  surface_holder_.destroyed().Connect(this, &SubSurface::OnSurfaceDestroyed);
  wl_sub_surface_.Setup(Display::wl_subcompositor(),
                        surface_holder_.wl_surface(),
                        surface_holder_.wl_surface(parent));

  surface_holder_.SetParent(parent);
}

SubSurface::~SubSurface() {

}

void SubSurface::PlaceAbove(ViewSurface *sibling) {
  if (sibling == surface_holder_.view_surface()) return;

  if (surface_holder_.view_surface()->parent() == sibling->parent() ||
      surface_holder_.view_surface() == sibling->parent() ||
      surface_holder_.view_surface()->parent() == sibling) {
    wl_sub_surface_.PlaceAbove(surface_holder_.wl_surface(sibling));
    surface_holder_.MoveAbove(sibling);
  }
}

void SubSurface::PlaceBelow(ViewSurface *sibling) {
  if (sibling == surface_holder_.view_surface()) return;

  if (surface_holder_.view_surface()->parent() == sibling->parent() ||
      surface_holder_.view_surface() == sibling->parent() ||
      surface_holder_.view_surface()->parent() == sibling) {
    wl_sub_surface_.PlaceBelow(surface_holder_.wl_surface(sibling));
    surface_holder_.MoveBelow(sibling);
  }
}

void SubSurface::SetRelativePosition(int x, int y) {
  wl_sub_surface_.SetPosition(x, y);
  surface_holder_.SetRelativePosition(x, y);
}

void SubSurface::SetWindowPosition(int x, int y) {
  Point parent_global_position = surface_holder_.view_surface()->parent()->GetWindowPosition();
  wl_sub_surface_.SetPosition(x - parent_global_position.x - surface_holder_.view_surface()->margin().l,
                              y - parent_global_position.y - surface_holder_.view_surface()->margin().t);
}

void SubSurface::OnSurfaceDestroyed(SLOT) {
  wl_sub_surface_.Destroy();
}

}
