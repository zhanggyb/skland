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

#include "../wayland/subsurface.hpp"

namespace skland {

class AbstractSurface;

/**
 * @ingroup gui
 * @brief Sub surface role
 */
class SubSurface {

 public:

  SubSurface();

  ~SubSurface();

  /**
   * @brief Assign sub surface role to the given new created surface
   * @param surface
   * @param parent
   */
  void Setup(AbstractSurface *surface, AbstractSurface *parent);

  void Destroy();

 private:

  void OnSurfaceDestroyed();

  AbstractSurface *surface_;

  wayland::SubSurface wl_sub_surface_;

};

}

#endif // SKLAND_GUI_SUB_SURFACE_HPP_
