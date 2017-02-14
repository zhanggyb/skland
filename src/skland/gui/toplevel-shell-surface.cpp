//
// Created by zhanggyb on 17-2-14.
//

#include <skland/gui/toplevel-shell-surface.hpp>

namespace skland {

ToplevelShellSurface::ToplevelShellSurface(AbstractView *view, const Margin &margin)
    : ShellSurface(view, margin) {
  surface_holder().destroyed().Connect(this, &ToplevelShellSurface::OnSurfaceDestroyed, -1);
  xdg_toplevel_.Setup(xdg_surface());
}

ToplevelShellSurface::~ToplevelShellSurface() {

}

void ToplevelShellSurface::OnSurfaceDestroyed(SLOT) {
  xdg_toplevel_.Destroy();
}

}
