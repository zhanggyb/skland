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
#include "memory-pool.hpp"
#include "buffer.hpp"

#include "../stock/theme.hpp"

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
SKLAND_EXPORT class Window : public AbstractShellView {

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

 public:

  Window(const char *title, AbstractShellFrame *frame = Theme::CreateWindowFrame());

  Window(int width, int height, const char *title,
         AbstractShellFrame *frame = Theme::CreateWindowFrame());

  virtual ~Window();

  void SetContentView(AbstractView *view);

 protected:

  virtual void OnShown() final;

  virtual void OnUpdate(AbstractView *view) override;

  virtual Surface *GetSurface(const AbstractView *view) const;

  virtual void OnKeyboardKey(KeyEvent *event) final;

  virtual void OnSizeChanged(int old_width, int old_height, int new_width, int new_height) final;

 private:

  Surface *main_surface_;

  /* Properties for frame surface, JUST experimental */
  MemoryPool frame_pool_;
  Buffer frame_buffer_;
  std::shared_ptr<Canvas> frame_canvas_;

  /* Properties for main surface, JUST experimental */
  MemoryPool main_pool_;
  Buffer main_buffer_;
  std::shared_ptr<Canvas> main_canvas_;

};

}

#endif // SKLAND_GUI_WINDOW_HPP_
