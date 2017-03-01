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

#ifndef SKLAND_GUI_SHELL_SURFACE_HPP_
#define SKLAND_GUI_SHELL_SURFACE_HPP_

#include "../core/margin.hpp"
#include "../wayland/xdg-surface.hpp"

namespace skland {

class AbstractEventHandler;
class Surface;
class ToplevelShellSurface;
class PopupShellSurface;

/**
 * @ingroup gui
 * @brief Shell surface role
 */
class ShellSurface {

  friend class Surface;
  friend class ToplevelShellSurface;
  friend class PopupShellSurface;

  ShellSurface() = delete;
  ShellSurface(const ShellSurface &) = delete;
  ShellSurface &operator=(const ShellSurface &) = delete;

 public:

  static ShellSurface *Get(const Surface *surface);

  void ResizeWindow(int width, int height) const;

  void AckConfigure(uint32_t serial) const {
    xdg_surface_.AckConfigure(serial);
  }

  Surface *surface() const { return surface_; }

  DelegateRef<void(uint32_t)> configure() { return xdg_surface_.configure(); }

 private:

  static Surface *Create(AbstractEventHandler *event_handler, const Margin &margin = Margin());

  ShellSurface(Surface *surface);

  ~ShellSurface();

  void PushShellSurface();

  void RemoveShellSurface();

  Surface *surface_;

  wayland::XdgSurface xdg_surface_;

  ShellSurface *parent_;

  union {
    void *placeholder;
    ToplevelShellSurface *toplevel_shell_surface;
    PopupShellSurface *popup_shell_surface_;
  } role_;

};

}

#endif // SKLAND_GUI_SHELL_SURFACE_HPP_
