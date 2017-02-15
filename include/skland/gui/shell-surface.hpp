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

#include "view-surface-holder.hpp"
#include "../wayland/xdg-surface.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief Shell surface role
 */
class ShellSurface : public Trackable {

  ShellSurface() = delete;
  ShellSurface(const ShellSurface &) = delete;
  ShellSurface &operator=(const ShellSurface &) = delete;

 public:

  ShellSurface(AbstractView *view, const Margin &margin = Margin());

  virtual ~ShellSurface();

  DelegateRef<void(uint32_t)> surface_configure() { return xdg_surface_.configure(); }

  ViewSurface *view_surface() const { return view_surface_holder_.view_surface(); }

 protected:

  const ViewSurfaceHolder &view_surface_holder() const { return view_surface_holder_; }

  const wayland::XdgSurface &xdg_surface() const { return xdg_surface_; }

  SignalRef<> destroying() { return destroying_; }

 private:

  void OnViewSurfaceDestroying(__SLOT__);

  ViewSurfaceHolder view_surface_holder_;

  wayland::XdgSurface xdg_surface_;

  Signal<> destroying_;

};

}

#endif // SKLAND_GUI_SHELL_SURFACE_HPP_
