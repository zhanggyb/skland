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

#ifndef SKLAND_GUI_TITLE_BAR_HPP_
#define SKLAND_GUI_TITLE_BAR_HPP_

#include "abstract-button.hpp"

#include "../core/color.hpp"
#include "../graphic/font.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief The default title bar on the top of window
 */
class TitleBar final : public AbstractView {

 public:

  enum ButtonType {
    kButtonClose,
    kButtonMaximize,
    kButtonMinimize,
    kButtonFullscreen,
    kButtonMenu
  };

  TitleBar();

  virtual ~TitleBar();

  void SetForeground(const Color &color);

  void SetTitle(const std::string &title);

  AbstractButton *GetButton(ButtonType button_type) const;

 protected:

  virtual void OnGeometryWillChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override;

  virtual void OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override;

  virtual void OnLayout(int dirty_flag, int left, int top, int right, int bottom) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

 private:

  class Button;
  class CloseButton;
  class MaximizeButton;
  class MinimizeButton;
  class FullscreenButton;

  static const int kButtonSpace = 5;

  CloseButton *close_button_;
  MaximizeButton *maximize_button_;
  MinimizeButton *minimize_button_;
  FullscreenButton *fullscreen_button_;

  Font font_;

  Color foreground_;

  std::string title_;
};

}

#endif // SKLAND_GUI_TITLE_BAR_HPP_
