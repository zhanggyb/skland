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

namespace skland {

class Display;
class Application;
class AbstractWindowFrame;

enum WindowFlagMask {
  kWindowFramelessMask = 0x1,
  kWindowModeMask = 0x3 << 1
};

enum WindowFlags {
  WindowFrameless = 0x1,
  WindowFullscreen = 0x1 << 1,
  WindowMaximized = 0x2 << 1,
  WindowMinimized = 0x3 << 1
};

class AbstractWindow : public AbstractView {

  friend class Display;
  friend class Application;
  friend class AbstractWindowFrame;

 public:

  AbstractWindow(const char *title, int flags = 0);

  AbstractWindow(int width, int height, const char *title, int flags = 0);

  virtual ~AbstractWindow();

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

 protected:

  void AddSubView(AbstractView *view, int pos = 0);

 private:

  Display *display_;  /**< The manager object */

  std::string title_;

  int flags_;
};

}

#endif // SKLAND_GUI_ABSTRACT_WINDOW_HPP_
