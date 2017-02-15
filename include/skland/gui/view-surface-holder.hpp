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

#ifndef SKLAND_GUI_VIEW_SURFACE_HOLDER_HPP_
#define SKLAND_GUI_VIEW_SURFACE_HOLDER_HPP_

#include "view-surface.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief The basic class acts as a surface role
 */
class ViewSurfaceHolder: public Trackable {

  ViewSurfaceHolder() = delete;

 public:

  ViewSurfaceHolder(AbstractView *view, const Margin &margin = Margin());

  ViewSurfaceHolder(ViewSurface *surface);

  ViewSurfaceHolder(const ViewSurfaceHolder &other);

  ~ViewSurfaceHolder();

  ViewSurfaceHolder &operator=(const ViewSurfaceHolder &other);

  ViewSurface *view_surface() const { return view_surface_; }

  SignalRef<> view_surface_destroying() { return view_surface_destroying_; }

  const wayland::Surface &wl_surface() const { return view_surface_->wl_surface_; }

  void SetParent(ViewSurface *parent);

  /**
   * @brief Move the local surface list and insert above target dst surface
   * @param dst
   */
  void MoveAbove(ViewSurface *dst);

  /**
   * @brief Move the local surface list and insert below target dst surface
   * @param dst
   */
  void MoveBelow(ViewSurface *dst);

  void SetRelativePosition(int x, int y) {
    view_surface_->relative_position_.x = x;
    view_surface_->relative_position_.y = y;
  }

  void PushShellSurface();

  void RemoveShellSurface();

  static const wayland::Surface &wl_surface(const ViewSurface *view_surface) {
    return view_surface->wl_surface_;
  }

  static int GetShellSurfaceCount() { return ViewSurface::kShellSurfaceCount; }

 private:

  void InsertAbove(ViewSurface *sibling);

  void InsertBelow(ViewSurface *sibling);

  void OnViewSurfaceDestroying(__SLOT__);

  ViewSurface *view_surface_;

  Signal<> view_surface_destroying_;

};

}

#endif // SKLAND_GUI_VIEW_SURFACE_HOLDER_HPP_
