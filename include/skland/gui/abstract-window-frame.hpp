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

#ifndef SKLAND_GUI_ABSTRACT_WINDOW_FRAME_HPP_
#define SKLAND_GUI_ABSTRACT_WINDOW_FRAME_HPP_

#include <skland/core/sigcxx.hpp>
#include <skland/core/margin.hpp>
#include <skland/core/point.hpp>
#include <skland/core/rect.hpp>

#include <vector>

namespace skland {

class MouseEvent;
class AbstractWindow;
class AbstractWidget;
class Canvas;

/**
 * @brief The abstract class for window frame
 *
 * A window frame provides decoration on a window, and draw background and border.
 */
class AbstractWindowFrame : public Trackable {

  friend class AbstractWindow;

  AbstractWindowFrame(const AbstractWindowFrame &) = delete;
  AbstractWindowFrame &operator=(const AbstractWindowFrame &) = delete;

 public:

  enum TitleBarPosition {
    kTitleBarNone,
    kTitleBarTop,
    kTitleBarRight,
    kTitleBarBottom,
    kTitleBarLeft
  };

  AbstractWindowFrame();

  virtual ~AbstractWindowFrame();

  Rect GetClientGeometry() const;

  AbstractWindow *window() const {
    return window_;
  }

  int border() const {
    return border_;
  }

  TitleBarPosition title_bar_position() const {
    return title_bar_position_;
  }

  int title_bar_size() const {
    return title_bar_size_;
  }

  SignalRef<int> window_action() {
    return window_action_;
  }

  /** The margin within which the cursor should switch to resizing,
   * the values SHOULD smaller than shadow margin */
  static const Margin kResizingMargin;

 protected:

  virtual void OnSetup() = 0;

  virtual void OnResize(int width, int height) = 0;

  virtual void OnDraw(Canvas *context) = 0;

  virtual int GetMouseLocation(const MouseEvent *event) const = 0;

  void AddWidget(AbstractWidget *widget, int pos = 0);

  void EmitAction(int action) {
    window_action_.Emit(action);
  }

  void set_border(int border) {
    border_ = border;
  }

  void set_title_bar_size(int title_bar_size) {
    title_bar_size_ = title_bar_size;
  }

  void set_title_bar_position(TitleBarPosition position) {
    title_bar_position_ = position;
  }

 private:

  AbstractWindow *window_;

  int border_;
  int title_bar_size_;
  TitleBarPosition title_bar_position_;

  Signal<int> window_action_;
};

}

#endif // SKLAND_ABSTRACT_WINDOW_FRAME_HPP_
