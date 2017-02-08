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

#ifndef SKLAND_STOCK_INTERNAL_WINDOW_FRAME_DARK_HPP_
#define SKLAND_STOCK_INTERNAL_WINDOW_FRAME_DARK_HPP_

#include <skland/gui/abstract-window-frame.hpp>

#include <skland/core/color.hpp>
#include <skland/gui/abstract-button.hpp>

namespace skland {

class Label;

/**
 * @brief The default window frame
 */
class WindowFrameDark final : public AbstractWindowFrame {

  WindowFrameDark(const WindowFrameDark &orig) = delete;
  WindowFrameDark &operator=(const WindowFrameDark &other) = delete;

 public:

  enum Mode {
    kModeLight,
    kModeDark
  };

  WindowFrameDark();

  virtual ~WindowFrameDark();

 protected:

  virtual void OnSetup();

  virtual void OnResize(int width, int height);

  virtual void OnDraw(const Context *context);

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
  virtual int GetMouseLocation(const MouseEvent *event) const;

 private:

/// @cond IGNORE

  class CloseButton final : public AbstractButton {

   public:

    CloseButton();

    virtual ~CloseButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(const Context *context) final;

  };

  class MaximizeButton final : public AbstractButton {

   public:

    MaximizeButton();

    virtual ~MaximizeButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(const Context *context) final;

  };

  class MinimizeButton final : public AbstractButton {

   public:

    MinimizeButton();

    virtual ~MinimizeButton();

    virtual Size GetPreferredSize() const final;

   protected:

    virtual void OnResize(int width, int height) final;

    virtual void OnDraw(const Context *context) final;

  };

/// @endcond

  void OnCloseButtonClicked(__SLOT__);

  void OnMaximizeButtonClicked(__SLOT__);

  void OnMinimizeButtonClicked(__SLOT__);

  void CreateWidgets();

  void LayoutWidgets(int width, int height);

  void DrawShadow(Canvas *canvas);

  CloseButton *close_button_;

  MaximizeButton *maximize_button_;

  MinimizeButton *minimize_button_;

  Label *title_;

};

} // namespace skland

extern "C" void *WindowFrameDarkCreate();

extern "C" void WindowFrameDarkDestroy(void *p);

#endif // SKLAND_STOCK_INTERNAL_WINDOW_FRAME_DARK_HPP_
