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

#ifndef SKLAND_EGL_WINDOW_HPP
#define SKLAND_EGL_WINDOW_HPP

#include "abstract-view.hpp"

#include "../wayland/xdg-surface.hpp"
#include "../wayland/xdg-toplevel.hpp"
#include "../wayland/region.hpp"
#include "../wayland/callback.hpp"

namespace skland {

class EGLSurface;

class EGLWindow : public AbstractView {

 public:

  EGLWindow();

  EGLWindow(int width, int height);

  virtual ~EGLWindow();

  void Show();

  void Close(__SLOT__);

  virtual Size GetMinimalSize() const override;

  virtual Size GetPreferredSize() const override;

  virtual Size GetMaximalSize() const override;

 protected:

  virtual void OnInitializeEGL();

  virtual void OnResizeEGL(int width, int height);

  virtual void OnRenderEGL();

  virtual void OnUpdate(AbstractView *view) final;

  virtual AbstractSurface *OnGetSurface(const AbstractView *view) const final;

  virtual void OnResize(int width, int height) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) final;

 private:

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  void OnFrame(uint32_t serial);

  bool is_xdg_surface_configured_;

  wayland::XdgSurface xdg_surface_;
  wayland::XdgToplevel xdg_toplevel_;

  wayland::Region input_region_;
  wayland::Region inactive_region_;

  EGLSurface *surface_;

  wayland::Callback frame_callback_;
};

}

#endif //SKLAND_EGL_WINDOW_HPP
