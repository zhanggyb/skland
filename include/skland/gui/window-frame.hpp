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

#ifndef SKLAND_GUI_WINDOW_FRAME_HPP_
#define SKLAND_GUI_WINDOW_FRAME_HPP_

#include "abstract-window-frame.hpp"

#include "../core/color.hpp"
#include "abstract-button.hpp"

namespace skland {

class Label;

/**
 * @brief The default window frame
 */
class WindowFrame : public AbstractWindowFrame {

  WindowFrame() = delete;
  WindowFrame(const WindowFrame &orig) = delete;
  WindowFrame &operator=(const WindowFrame &other) = delete;

 public:

  WindowFrame(AbstractWindow *window,
              int border = 0,
              TitleBarPosition title_bar_position = kTitleBarTop,
              int title_bar_size = 22);

  virtual ~WindowFrame();

  virtual void Resize(int width, int height) final;

  virtual void Draw(Canvas *canvas) final;

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
  virtual int GetMouseLocation(const MouseEvent *event) const override;

 private:

/// @cond IGNORE

  class CloseButton : public AbstractButton {

   public:

    CloseButton();

    virtual ~CloseButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(Canvas *canvas) final;

  };

  class MaximizeButton : public AbstractButton {

   public:

    MaximizeButton();

    virtual ~MaximizeButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(Canvas *canvas) final;

  };

  class MinimizeButton : public AbstractButton {

   public:

    MinimizeButton();

    virtual ~MinimizeButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(Canvas *canvas) final;

  };

/// @endcond

  void OnCloseButtonClicked(__SLOT__);

  void OnMaximizeButtonClicked(__SLOT__);

  void OnMinimizeButtonClicked(__SLOT__);

  void CreateWidgets();

  void LayoutWidgets(int width, int height);

  CloseButton *close_button_;

  MaximizeButton *maximize_button_;

  MinimizeButton *minimize_button_;

  Label *title_;

  Color foreground_;

  Color background_;
};

}

#endif // SKLAND_GUI_WINDOW_FRAME_HPP_
