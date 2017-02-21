//
// Created by zhanggyb on 17-2-21.
//

#ifndef SKLAND_GUI_INTERNAL_THEME_DEFAULT_HPP_
#define SKLAND_GUI_INTERNAL_THEME_DEFAULT_HPP_

#include <skland/core/color.hpp>

#include <skland/gui/abstract-window-frame.hpp>
#include <skland/gui/abstract-button.hpp>

namespace skland {

class Canvas;
class Label;

class CloseButton final : public AbstractButton {

 public:

  CloseButton();

  virtual ~CloseButton();

  virtual Size GetPreferredSize() const;

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

 protected:

  virtual void OnResize(int width, int height);

  virtual void OnDraw(const Context *context);

 private:

  Color foreground_;
  Color background_;

};

class MaximizeButton final : public AbstractButton {

 public:

  MaximizeButton();

  virtual ~MaximizeButton();

  virtual Size GetPreferredSize() const;

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

 protected:

  virtual void OnResize(int width, int height);

  virtual void OnDraw(const Context *context);

 private:

  Color foreground_;
  Color background_;

};

class MinimizeButton final : public AbstractButton {

 public:

  MinimizeButton();

  virtual ~MinimizeButton();

  virtual Size GetPreferredSize() const;

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

 protected:

  virtual void OnResize(int width, int height);

  virtual void OnDraw(const Context *context);

 private:

  Color foreground_;
  Color background_;

};

class WindowFrameDefault : public AbstractWindowFrame {

  WindowFrameDefault(const WindowFrameDefault &) = delete;
  WindowFrameDefault &operator=(const WindowFrameDefault &) = delete;

 public:

  WindowFrameDefault();

  virtual ~WindowFrameDefault();

  static const int kButtonSize = 14;
  static const int kButtonSpace = 5;

 protected:

  virtual void OnSetup() override;

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
  virtual int GetMouseLocation(const MouseEvent *event) const;

  void DrawShadow(Canvas *canvas);

  CloseButton *close_button() const { return close_button_; }

  MaximizeButton *maximize_button() const { return maximize_button_; }

  MinimizeButton *minimize_button() const { return minimize_button_; }

  Label *title() const { return title_; }

 private:

  void OnCloseButtonClicked(__SLOT__);

  void OnMaximizeButtonClicked(__SLOT__);

  void OnMinimizeButtonClicked(__SLOT__);

  void CreateWidgets();

  void LayoutWidgets(int width, int height);

  CloseButton *close_button_;

  MaximizeButton *maximize_button_;

  MinimizeButton *minimize_button_;

  Label *title_;

};

}

#endif // SKLAND_GUI_INTERNAL_THEME_DEFAULT_HPP_
