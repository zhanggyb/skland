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
#include "abstract-window-frame.hpp"

#include "../core/defines.hpp"
#include "../core/rect.hpp"

#include "../wayland/xdg-surface.hpp"
#include "../wayland/xdg-toplevel.hpp"

#include "../stock/theme.hpp"

#include <cstdint>
#include <string>

namespace skland {

class AbstractWindowFrame;
class ShmSurface;

enum WindowFlags {
  kWindowFullscreen = 0x1,
  kWindowMaximized = 0x2,
  kWindowMinimized = 0x3,
};

/**
 * @ingroup gui
 * @brief Abstract class for top level windows
 */
class AbstractWindow : public AbstractView {

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

  void SetAppId(const char *app_id);

  void SetWindowFrame(AbstractWindowFrame *window_frame);

  void Show();

  void Close(__SLOT__);

  void Maximize(__SLOT__);

  void Minimize(__SLOT__);

  const std::string &title() const { return title_; }

  int flags() const { return flags_; }

  virtual Size GetMinimalSize() const override;

  virtual Size GetPreferredSize() const override;

  virtual Size GetMaximalSize() const override;

  bool IsFullscreened() const { return kWindowFullscreen == flags_; }

  bool IsMaximized() const { return kWindowMaximized == flags_; }

  bool IsMinimized() const { return kWindowMinimized == flags_; }

  bool IsFrameless() const { return nullptr == window_frame_; }

  int GetMouseLocation(const MouseEvent *event) const;

  Rect GetClientGeometry() const;

 protected:

  virtual void OnShown() = 0;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

  void AddSubView(AbstractView *view, int pos = -1);

  void MoveWithMouse(MouseEvent *event) const;

  void ResizeWithMouse(MouseEvent *event, uint32_t edges) const;

  void SetShellSurface(AbstractSurface *surface);

  AbstractWindowFrame *window_frame() const {
    return window_frame_;
  }

  AbstractSurface *shell_surface() const {
    return shell_surface_;
  }

  static void ResizeWindowFrame(AbstractWindowFrame *window_frame, int width, int height) {
    window_frame->OnResize(width, height);
  }

 private:

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  void OnWindowAction(int action, __SLOT__);

  int flags_;

  AbstractWindowFrame *window_frame_;

  AbstractSurface *shell_surface_;
  wayland::XdgSurface xdg_surface_;
  wayland::XdgToplevel xdg_toplevel_;

  std::string title_;
  std::string app_id_;

  Size saved_size_;

  bool is_xdg_surface_configured_;

};

}

#endif // SKLAND_GUI_ABSTRACT_WINDOW_HPP_
