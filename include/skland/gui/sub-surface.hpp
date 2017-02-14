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

#ifndef SKLAND_GUI_SUB_SURFACE_HPP_
#define SKLAND_GUI_SUB_SURFACE_HPP_

#include "surface-holder.hpp"
#include "../wayland/subsurface.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief Sub surface role
 */
class SubSurface : public Trackable {

  SubSurface() = delete;
  SubSurface(const SubSurface &) = delete;
  SubSurface &operator=(const SubSurface &)= delete;

 public:

  SubSurface(ViewSurface *parent, AbstractView *view, const Margin &margin = Margin());

  virtual ~SubSurface();

  void PlaceAbove(ViewSurface *sibling);

  void PlaceBelow(ViewSurface *sibling);

  void SetRelativePosition(int x, int y);

  void SetWindowPosition(int x, int y);

  ViewSurface *view_surface() const { return surface_holder_.view_surface(); }

 private:

  void OnSurfaceDestroyed(__SLOT__);

  SurfaceHolder surface_holder_;

  wayland::SubSurface wl_sub_surface_;

};

}

#endif // SKLAND_GUI_SUB_SURFACE_HPP_
