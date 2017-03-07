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

#ifndef SKLAND_GUI_POPUP_SHELL_SURFACE_HPP_
#define SKLAND_GUI_POPUP_SHELL_SURFACE_HPP_

#include "../core/margin.hpp"
#include "../wayland/xdg-popup.hpp"
#include "../wayland/xdg-positioner.hpp"

namespace skland {

class Surface;
class AbstractEventHandler;
class ShellSurface;

/**
 * @ingroup gui
 * @brief Xdg popup shell surface
 */
class PopupShellSurface {

  friend class ShellSurface;

  PopupShellSurface() = delete;
  PopupShellSurface(const PopupShellSurface &) = delete;
  PopupShellSurface &operator=(const PopupShellSurface &) = delete;

 public:

  static Surface *Create(ShellSurface *parent, AbstractEventHandler *view, const Margin &margin = Margin());

  PopupShellSurface(AbstractEventHandler *view, const Margin &margin = Margin());

 private:

  PopupShellSurface(ShellSurface *shell_surface);

  ~PopupShellSurface();

  ShellSurface *shell_surface_;

  wayland::XdgPopup xdg_popup_;
  wayland::XdgPositioner xdg_positioner_;

};

}

#endif // SKLAND_GUI_POPUP_SHELL_SURFACE_HPP_
