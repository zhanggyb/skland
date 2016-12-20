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

class Window : public AbstractWindow {

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

 public:

  Window(const char *title, AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  Window(int width, int height, const char *title,
         AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  virtual ~Window();

  void SetMainWidget(AbstractWidget *widget);

  AbstractWidget *main_widget() const {
    return main_widget_;
  }

 protected:

  virtual void OnKeyboardKey(KeyEvent *event) final;

  virtual void OnResize(int width, int height) final;

  virtual void OnCanvasSetup() final;

 private:

  void SetMainWidgetGeometry();

  MemoryPool pool_;
  Buffer buffer_;

  AbstractWidget *main_widget_;
};

}

#endif // SKLAND_GUI_WINDOW_HPP_
