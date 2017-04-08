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

class Canvas;
class Label;

/**
 * @ingroup stock_intern
 * @brief Close button on title bar
 */
class CloseButton final : public AbstractButton {

 public:

  CloseButton();

  virtual ~CloseButton();

  virtual Size GetPreferredSize() const;

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

 protected:

  virtual void OnDraw(const Context *context);

 private:

  static const int kButtonSize = 14;

  Color foreground_;
  Color background_;

};

/**
 * @ingroup stock_intern
 * @brief Maximize button on title bar
 */
class MaximizeButton final : public AbstractButton {

 public:

  MaximizeButton();

  virtual ~MaximizeButton();

  virtual Size GetPreferredSize() const;

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

 protected:

  virtual void OnDraw(const Context *context);

 private:

  static const int kButtonSize = 14;

  Color foreground_;
  Color background_;

};

/**
 * @ingroup stock_intern
 * @brief Minimize button on title bar
 */
class MinimizeButton final : public AbstractButton {

 public:

  MinimizeButton();

  virtual ~MinimizeButton();

  virtual Size GetPreferredSize() const;

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

 protected:

  virtual void OnDraw(const Context *context);

 private:

  static const int kButtonSize = 14;

  Color foreground_;
  Color background_;

};

class TitleBar final : public AbstractView {

 public:

  TitleBar();

  virtual ~TitleBar();

  void SetForeground(const Color &color);

  void SetTitle(const std::string &title);

  CloseButton *close_button() const { return close_button_; }

  MaximizeButton *maximize_button() const { return maximize_button_; }

  MinimizeButton *minimize_button() const { return minimize_button_; }

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

  static const int kButtonSpace = 5;

  CloseButton *close_button_;
  MaximizeButton *maximize_button_;
  MinimizeButton *minimize_button_;

  Font font_;

  Color foreground_;

  std::string title_;
};

}

#endif // SKLAND_GUI_TITLE_BAR_HPP_
