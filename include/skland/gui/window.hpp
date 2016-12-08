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

#ifndef SKLAND_GUI_WINDOW_HPP_
#define SKLAND_GUI_WINDOW_HPP_

#include "abstract-window.hpp"
#include "memory-pool.hpp"
#include "buffer.hpp"

#include <skland/wayland/client/xdg-surface.hpp>
#include <skland/wayland/client/xdg-toplevel.hpp>
#include <skland/wayland/client/region.hpp>

namespace skland {

class AbstractWindowFrame;
class AbstractWidget;

class Window : public AbstractWindow {

 public:

  Window(const char *title, int flags = 0);

  Window(int width, int height, const char *title, int flags = 0);

  virtual ~Window();

  void SetMainWidget(AbstractWidget *widget);

  AbstractWidget *main_widget() const {
    return main_widget_;
  }

  virtual Size GetMinimalSize() const final;

 protected:

  virtual void OnMouseEnter(MouseEvent *event) final;

  virtual void OnMouseLeave(MouseEvent *event) final;

  virtual void OnMouseMove(MouseEvent *event) final;

  virtual void OnMouseButton(MouseEvent *event) final;

  virtual void OnKeyboardKey(KeyEvent *event) final;

  virtual void OnResize(int width, int height) final;

  virtual void OnDraw(Canvas *canvas) final;

 private:

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  void SetMainWidgetGeometry();

  wayland::client::XdgSurface xdg_surface_;
  wayland::client::XdgToplevel xdg_toplevel_;
  wayland::client::Region input_region_;

  MemoryPool pool_;
  Buffer buffer_;

  bool maximized_;
  bool minimized_;
  bool fullscreened_;

  AbstractWindowFrame *window_frame_;
  AbstractWidget *main_widget_;

  Size saved_size_;
};

}

#endif // SKLAND_GUI_WINDOW_HPP_
