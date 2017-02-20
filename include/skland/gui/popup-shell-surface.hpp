//
// Created by zhanggyb on 17-2-20.
//

#ifndef SKLAND_GUI_POPUP_SHELL_SURFACE_HPP_
#define SKLAND_GUI_POPUP_SHELL_SURFACE_HPP_

#include "shell-surface.hpp"
#include "../wayland/xdg-popup.hpp"
#include "../wayland/xdg-positioner.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief Xdg popup shell surface
 */
class PopupShellSurface : public ShellSurface {

  PopupShellSurface() = delete;
  PopupShellSurface(const PopupShellSurface &) = delete;
  PopupShellSurface &operator=(const PopupShellSurface &) = delete;

 public:

  PopupShellSurface(AbstractView *view, const Margin &margin = Margin());

  virtual ~PopupShellSurface();

 private:

  void OnShellSurfaceDestroying(__SLOT__);

  wayland::XdgPopup xdg_popup_;
  wayland::XdgPositioner xdg_positioner_;

};

}

#endif // SKLAND_GUI_POPUP_SHELL_SURFACE_HPP_
