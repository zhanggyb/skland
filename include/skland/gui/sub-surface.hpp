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

#include "../core/margin.hpp"
#include "../wayland/subsurface.hpp"

namespace skland {

class Surface;
class AbstractEventHandler;

/**
 * @ingroup gui
 * @brief Sub surface role
 */
class SubSurface {

  friend class Surface;

  SubSurface() = delete;
  SubSurface(const SubSurface &) = delete;
  SubSurface &operator=(const SubSurface &)= delete;

 public:

  static Surface *Create(Surface *parent, AbstractEventHandler *view, const Margin &margin = Margin());

  static SubSurface *Get(const Surface *surface);

  void PlaceAbove(Surface *sibling);

  void PlaceBelow(Surface *sibling);

  void SetRelativePosition(int x, int y);

  void SetWindowPosition(int x, int y);

  Surface *surface() const { return surface_; }

 private:

  SubSurface(Surface *surface, Surface *parent);

  ~SubSurface();

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

  void InsertAbove(Surface *sibling);

  void InsertBelow(Surface *sibling);

  Surface *surface_;

  wayland::SubSurface wl_sub_surface_;

};

}

#endif // SKLAND_GUI_SUB_SURFACE_HPP_
