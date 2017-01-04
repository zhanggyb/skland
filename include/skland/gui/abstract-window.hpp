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

#include "../core/defines.hpp"
#include "../core/rect.hpp"

#include "memory-pool.hpp"
#include "buffer.hpp"
#include "task.hpp"

#include <cstdint>
#include <string>

#include <skland/wayland/xdg-surface.hpp>
#include <skland/wayland/xdg-toplevel.hpp>
#include <skland/wayland/region.hpp>

#include <skland/stock/theme.hpp>

namespace skland {

class Display;
class Application;
class AbstractWindowFrame;
class Input;
class RasterSurface;

enum WindowFlags {
  kWindowFullscreen = 0x1,
  kWindowMaximized = 0x2,
  kWindowMinimized = 0x3,
};

class AbstractWindow : public AbstractView {

  friend class Display;
  friend class Application;
  friend class AbstractWindowFrame;
  friend class Input;

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

  virtual void OnUpdate(AbstractView *view) override;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnDraw(Canvas *canvas) override;

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

  wayland::XdgSurface xdg_surface_;

  wayland::XdgToplevel xdg_toplevel_;

  wayland::Region input_region_;
  wayland::Region frame_region_;

  std::string title_;

  std::string app_id_;

  Size saved_size_;

  bool is_xdg_surface_configured_;

  /**
   * @brief The main raster surface for rendering widgets
   */
  RasterSurface *main_surface_;

  /**
   * @brief The surface for frame
   *
   * FIXME:
   *
   * My initial indention is to use this surface for frame and background for this window.
   * It is a sub surface of the main surface but I failed to place this below the main surface.
   *
   * The current code use this surface for widgets and the main surface for frame and background.
   */
  RasterSurface *frame_surface_;

  /* Properties for main surface, JUST experimental */
  MemoryPool main_pool_;
  Buffer main_buffer_;

  /* Properties for frame surface, JUST experimental */
  MemoryPool frame_pool_;
  Buffer frame_buffer_;

};

}

#endif // SKLAND_GUI_ABSTRACT_WINDOW_HPP_
