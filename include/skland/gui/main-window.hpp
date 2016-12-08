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

#ifndef SKLAND_GUI_MAIN_WINDOW_HPP_
#define SKLAND_GUI_MAIN_WINDOW_HPP_

#include "abstract-window.hpp"

#include <skland/wayland/client/xdg-surface.hpp>
#include <skland/wayland/client/xdg-toplevel.hpp>
#include <skland/wayland/client/region.hpp>

namespace skland {

class Surface;
class Output;
class MemoryPool;
class Buffer;
class ShellSurface;
class WindowFrame;
class AbstractWidget;
class Callback;

class MainWindow : public AbstractWindow {
 public:

  MainWindow(const char *title, int flags = 0);

  MainWindow(int width, int height, const char *title, int flags = 0);

  virtual ~MainWindow();

  void SetMainWidget(AbstractWidget *widget);

  AbstractWidget *main_widget() const {
    return main_widget_;
  }

 protected:

  virtual void OnMouseEnter(MouseEvent *event) final;

  virtual void OnMouseLeave(MouseEvent *event) final;

  virtual void OnMouseMove(MouseEvent *event) final;

  virtual void OnMouseButton(MouseEvent *event) final;

  virtual void OnKeyboardKey(KeyEvent *event) final;

  virtual void OnResize(int width, int height) final;

  virtual void OnDraw(Canvas *engine) final;

 private:

  static const int kPadding = 5;

  void Initialize();

  void OnFrameDone(uint32_t event);

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  void ResizeMainWidget(int width, int height);

//  Buffer *buffer_sub_;

  WindowFrame *window_frame_;

  wayland::client::XdgSurface xdg_surface_;
  wayland::client::XdgToplevel xdg_toplevel_;
  wayland::client::Region input_region_;
  wayland::client::Region opaque_region_;

  int resize_location_;
  MemoryPool *pool_;
  Buffer *buffer_;

  bool redraw_needed_;

  AbstractWidget *main_widget_;

};

}

#endif // SKLAND_GUI_MAIN_WINDOW_HPP_
