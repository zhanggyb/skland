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

namespace skland {

class AbstractWidget;
class Surface;
class Canvas;

/**
 * @ingroup gui
 * @brief A simple window with client-side decorations
 *
 * @example hello.cpp
 */
class Window : public AbstractWindow {

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

 public:

  Window(const char *title, AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  Window(int width, int height, const char *title,
         AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  virtual ~Window();

  void SetMainWidget(AbstractView *widget);

  AbstractView *main_widget() const { return main_widget_; }

 protected:

  virtual void OnShown() final;

  virtual void OnUpdate(AbstractView *view) override;

  virtual Surface *OnGetSurface(const AbstractView *view) const;

  virtual void OnKeyboardKey(KeyEvent *event) final;

  virtual void OnResize(int width, int height) final;

 private:

  void SetMainWidgetGeometry();

  Surface *main_surface_;

  /* Properties for frame surface, JUST experimental */
  MemoryPool frame_pool_;
  Buffer frame_buffer_;
  std::shared_ptr<Canvas> frame_canvas_;

  /* Properties for main surface, JUST experimental */
  MemoryPool main_pool_;
  Buffer main_buffer_;
  std::shared_ptr<Canvas> main_canvas_;

  AbstractView *main_widget_; // This will be a layout

};

}

#endif // SKLAND_GUI_WINDOW_HPP_
