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

#ifndef SKLAND_GUI_ABSTRACT_WINDOW_HPP_
#define SKLAND_GUI_ABSTRACT_WINDOW_HPP_

#include "abstract-view.hpp"

#include <wayland-client.h>
#include <cstdint>
#include <string>

#include <skland/core/defines.hpp>
#include <skland/core/rect.hpp>

#include <skland/wayland/client/xdg-surface.hpp>
#include <skland/wayland/client/xdg-toplevel.hpp>
#include <skland/wayland/client/region.hpp>

#include <skland/stock/theme.hpp>

namespace skland {

class Display;
class Application;
class AbstractWindowFrame;

enum WindowFlags {
  kWindowFullscreen = 0x1,
  kWindowMaximized = 0x2,
  kWindowMinimized = 0x3,
};

class AbstractWindow : public AbstractView {

  friend class Display;
  friend class Application;
  friend class AbstractWindowFrame;

 public:

  enum Action {
    kActionClose,
    kActionMaximize,
    kActionMinimize,
    kActionMenu,
    kActionLast
  };

  AbstractWindow(const char *title,
                 AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  AbstractWindow(int width, int height,
                 const char *title,
                 AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  virtual ~AbstractWindow();

  void SetTitle(const char *title);

  void SetWindowFrame(AbstractWindowFrame *window_frame);

  void Close(__SLOT__);

  void Maximize(__SLOT__);

  void Minimize(__SLOT__);

  const std::string &title() const {
    return title_;
  }

  int flags() const {
    return flags_;
  }

  virtual Size GetMinimalSize() const override;

  virtual Size GetPreferredSize() const override;

  virtual Size GetMaximalSize() const override;

  bool IsFullscreen() const {
    return flags_ == kWindowFullscreen;
  }

  bool IsMaximized() const {
    return flags_ == kWindowMaximized;
  }

  bool IsMinimized() const {
    return flags_ == kWindowMinimized;
  }

  bool IsFrameless() const {
    return window_frame_ == nullptr;
  }

  int GetMouseLocation(const MouseEvent *event) const;

  Rect GetClientGeometry() const;

 protected:

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnDraw(Canvas *canvas) override;

  // TODO: maybe need a better method name
  virtual void OnConfigureCanvas() = 0;

  void AddSubView(AbstractView *view, int pos = 0);

  void MoveWithMouse(MouseEvent *event) const;

  void ResizeWithMouse(MouseEvent *event, uint32_t edges) const;

 private:

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  void OnWindowAction(int action, __SLOT__);

  Display *display_;  /**< The manager object */

  AbstractWindowFrame *window_frame_;

  int flags_;

  wayland::client::XdgSurface xdg_surface_;

  wayland::client::XdgToplevel xdg_toplevel_;

  wayland::client::Region input_region_;

  std::string title_;

  Size saved_size_;
};

}

#endif // SKLAND_GUI_ABSTRACT_WINDOW_HPP_
