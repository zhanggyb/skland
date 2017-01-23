//
// Created by zhanggyb on 17-1-23.
//

#ifndef SKLAND_EGL_WINDOW_HPP
#define SKLAND_EGL_WINDOW_HPP

#include "abstract-view.hpp"

#include "../wayland/xdg-surface.hpp"
#include "../wayland/xdg-toplevel.hpp"
#include "../wayland/region.hpp"

namespace skland {

class EGLSurface;

class EGLWindow : public AbstractView {

 public:

  EGLWindow();

  virtual ~EGLWindow();

  void Show();

  virtual Size GetMinimalSize() const override;

  virtual Size GetPreferredSize() const override;

  virtual Size GetMaximalSize() const override;

 protected:

  virtual void OnUpdate(AbstractView *view) override;

  virtual AbstractSurface *OnGetSurface(const AbstractView *view) const;

  virtual void OnResize(int width, int height) override;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

 private:

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  bool is_xdg_surface_configured_;

  wayland::XdgSurface xdg_surface_;
  wayland::XdgToplevel xdg_toplevel_;

  wayland::Region input_region_;
  wayland::Region inactive_region_;

  EGLSurface *surface_;

};

}

#endif //SKLAND_EGL_WINDOW_HPP
