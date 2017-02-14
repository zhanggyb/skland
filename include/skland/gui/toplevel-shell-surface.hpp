//
// Created by zhanggyb on 17-2-14.
//

#ifndef SKLAND_TOPLEVEL_SHELL_SURFACE_HPP
#define SKLAND_TOPLEVEL_SHELL_SURFACE_HPP

#include "shell-surface.hpp"
#include "../wayland/xdg-toplevel.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief Xdg toplevel shell surface
 */
class ToplevelShellSurface : public ShellSurface {

 public:

  ToplevelShellSurface(AbstractView *view, const Margin &margin = Margin());

  virtual ~ToplevelShellSurface();

  DelegateRef<void(int, int, int)> xdg_toplevel_configure() { return xdg_toplevel_.configure(); }

  DelegateRef<void()> xdg_toplevel_close() { return xdg_toplevel_.close(); }

 private:

  void OnSurfaceDestroyed(__SLOT__);

  wayland::XdgToplevel xdg_toplevel_;

};

}

#endif //SKLAND_TOPLEVEL_SHELL_SURFACE_HPP
