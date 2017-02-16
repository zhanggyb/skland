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

#ifndef SKLAND_GUI_SURFACE_HOLDER_HPP_
#define SKLAND_GUI_SURFACE_HOLDER_HPP_

#include "surface.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief The basic class acts as a surface role
 */
class SurfaceHolder : public Trackable {

  SurfaceHolder() = delete;

 public:

  SurfaceHolder(AbstractView *view, const Margin &margin = Margin());

  SurfaceHolder(Surface *surface);

  SurfaceHolder(const SurfaceHolder &other);

  virtual ~SurfaceHolder();

  SurfaceHolder &operator=(const SurfaceHolder &other);

  Surface *surface() const { return surface_; }

  SignalRef<> surface_destroying() { return surface_destroying_; }

  const wayland::Surface &wl_surface() const { return surface_->wl_surface_; }

  void SetParent(Surface *parent);

  /**
   * @brief Move the local surface list and insert above target dst surface
   * @param dst
   */
  void MoveAbove(Surface *dst);

  /**
   * @brief Move the local surface list and insert below target dst surface
   * @param dst
   */
  void MoveBelow(Surface *dst);

  void SetRelativePosition(int x, int y) {
    surface_->relative_position_.x = x;
    surface_->relative_position_.y = y;
  }

  void PushShellSurface();

  void RemoveShellSurface();

  static const wayland::Surface &wl_surface(const Surface *surface) {
    return surface->wl_surface_;
  }

  static int GetShellSurfaceCount() { return Surface::kShellSurfaceCount; }

 private:

  void InsertAbove(Surface *sibling);

  void InsertBelow(Surface *sibling);

  void OnViewSurfaceDestroying(__SLOT__);

  Surface *surface_;

  Signal<> surface_destroying_;

};

}

#endif // SKLAND_GUI_VIEW_SURFACE_HOLDER_HPP_
