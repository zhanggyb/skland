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

#ifndef SKLAND_ABSTRACT_WINDOW_FRAME_HPP_
#define SKLAND_ABSTRACT_WINDOW_FRAME_HPP_

#include <skland/core/margin.hpp>
#include <skland/core/point.hpp>
#include <skland/core/sigcxx.hpp>

#include <vector>
#include <skland/core/size.hpp>
#include <skland/skland.hpp>

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

  AbstractWindowFrame() = delete;
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

  AbstractWindowFrame(AbstractWindow *window,
                      int border = 5,
                      TitleBarPosition title_bar_position = kTitleBarTop,
                      int title_bar_size = 30);

  virtual ~AbstractWindowFrame();

  Rect GetLocalClientGeometry() const;

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

  virtual void Resize(int width, int height) = 0;

  virtual void Draw(const Canvas *context) = 0;

  virtual int GetPointerLocation(const MouseEvent *event) const = 0;

  /** The shadow margin */
  static const Margin kShadowMargin;

  /** The margin within which the cursor should switch to resizing, the values SHOULD smaller than shadow margin */
  static const Margin kResizingMargin;

  static const Point kShadowOffset;

  static const int kShadowBlurRadius;

 protected:

  void AddWidget(AbstractWidget *widget, int pos = 0);

  void EmitAction(int action) {
    window_action_.Emit(action);
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
