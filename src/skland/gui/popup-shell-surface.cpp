//
// Created by zhanggyb on 17-2-20.
//

#include <skland/gui/popup-shell-surface.hpp>

namespace skland {

PopupShellSurface::PopupShellSurface(AbstractView *view, const Margin &margin)
    : ShellSurface(view, margin) {
  destroying().Connect(this, &PopupShellSurface::OnShellSurfaceDestroying);
}

PopupShellSurface::~PopupShellSurface() {
  UnbindAll();
  xdg_popup_.Destroy();
}

void PopupShellSurface::OnShellSurfaceDestroying(SLOT slot) {
  xdg_popup_.Destroy();
  Unbind(slot);
}

}