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

#ifndef SKLAND_GUI_INTERNAL_THEME_DEFAULT_HPP_
#define SKLAND_GUI_INTERNAL_THEME_DEFAULT_HPP_

#include <skland/core/color.hpp>

#include <skland/gui/abstract-shell-frame.hpp>
#include <skland/gui/abstract-button.hpp>

#include <skland/graphic/font.hpp>

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

  Color foreground_;
  Color background_;

};

class TitleBar final : public AbstractView {

 public:

  TitleBar();

  virtual ~TitleBar();

  virtual Size GetMinimalSize() const override;

  virtual Size GetPreferredSize() const override;

  virtual Size GetMaximalSize() const override;

  void SetForeground(const Color &color);

  void SetTitle(const std::string &title);

  CloseButton *close_button() const { return close_button_; }

  MaximizeButton *maximize_button() const { return maximize_button_; }

  MinimizeButton *minimize_button() const { return minimize_button_; }

 protected:

  virtual void OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

 private:

  CloseButton *close_button_;
  MaximizeButton *maximize_button_;
  MinimizeButton *minimize_button_;

  Font font_;

  Color foreground_;

  std::string title_;
};

/**
 * @ingroup stock_intern
 * @brief The default window frame
 */
class WindowFrameDefault : public AbstractShellFrame {

  WindowFrameDefault(const WindowFrameDefault &) = delete;
  WindowFrameDefault &operator=(const WindowFrameDefault &) = delete;

 public:

  WindowFrameDefault();

  virtual ~WindowFrameDefault();

  virtual Rect GetClientGeometry(int width, int height) const final;

  static const int kButtonSize = 14;
  static const int kButtonSpace = 5;

 protected:

  virtual void OnSetup() final;

  virtual void OnResize(int width, int height) override;

  virtual void OnDraw(const Context *context) override;

  /**
    * Calculate and return the cursor position at the border of this frame
    * @param x
    * @param y
    * @return
    *
    * |        |                 |
    * |        |                 |
    * | shadow | resizing border | resizing border
    * |        |                 |
    * |        |                 |
    */
  virtual int GetMouseLocation(const MouseEvent *event) const final;

  void DrawShadow(Canvas *canvas);

 private:

  void OnCloseButtonClicked(__SLOT__);

  void OnMaximizeButtonClicked(__SLOT__);

  void OnMinimizeButtonClicked(__SLOT__);

  int border_;
  int title_bar_size_;
  TitlePosition title_bar_position_;

  TitleBar *title_bar_;

};

}

#endif // SKLAND_GUI_INTERNAL_THEME_DEFAULT_HPP_
