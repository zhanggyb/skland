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

#include "abstract-shell-view.hpp"
#include "shared-memory-pool.hpp"
#include "buffer.hpp"

#include "../stock/theme.hpp"

namespace skland {

class AbstractWidget;
class Surface;
class Canvas;
class TitleBar;

/**
 * @ingroup gui
 * @brief A simple window with client-side decorations
 *
 * @example hello.cpp
 */
SKLAND_EXPORT class Window : public AbstractShellView {

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

 public:

  Window(const char *title);

  Window(int width, int height, const char *title);

  virtual ~Window();

  void SetContentView(AbstractView *view);

 protected:

  virtual void OnShown() final;

  virtual void OnUpdate(AbstractView *view) override;

  virtual Surface *GetSurface(const AbstractView *view) const;

  virtual void OnResize(int old_width, int old_height, int new_width, int new_height) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

  virtual void OnFocus(bool);

  virtual void RecursiveUpdate() override;

 private:

  virtual int GetMouseLocation(const MouseEvent *event) const;

  void DrawShadow(Canvas *canvas);

  void OnContentViewDestroyed(AbstractView *view, __SLOT__);

  void SetContentViewGeometry();

  static const Margin kResizingMargin;

  Surface *main_surface_;

  /* Properties for frame surface, JUST experimental */
  SharedMemoryPool pool_;

  Buffer frame_buffer_;
  std::shared_ptr<Canvas> frame_canvas_;

  /* Properties for main surface, JUST experimental */
  Buffer main_buffer_;
  std::shared_ptr<Canvas> main_canvas_;

  TitleBar *title_bar_;
  AbstractView *content_view_;

};

}

#endif // SKLAND_GUI_WINDOW_HPP_
