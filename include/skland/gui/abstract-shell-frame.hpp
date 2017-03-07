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

#include "../core/sigcxx.hpp"
#include "../core/margin.hpp"
#include "../core/point.hpp"
#include "../core/rect.hpp"

#include <vector>

namespace skland {

class MouseEvent;
class AbstractView;
class AbstractShellView;
class Context;

/**
 * @ingroup gui
 * @brief The abstract class for window frame
 *
 * A window frame provides decoration on a window, and draw background and border.
 */
class AbstractShellFrame : public Trackable {

  friend class AbstractShellView;

  AbstractShellFrame(const AbstractShellFrame &) = delete;
  AbstractShellFrame &operator=(const AbstractShellFrame &) = delete;

 public:

  enum TitleBarPosition {
    kTitleBarNone,
    kTitleBarTop,
    kTitleBarRight,
    kTitleBarBottom,
    kTitleBarLeft
  };

  AbstractShellFrame();

  virtual ~AbstractShellFrame();

  void SetTitleBar(AbstractView *view);

  virtual Rect GetContentGeometry() const = 0;

  AbstractView *GetTitleView() const { return title_view_; }

  SignalRef<int> window_action() { return window_action_; }

  /** The margin within which the cursor should switch to resizing,
   * the values SHOULD smaller than shadow margin */
  static const Margin kResizingMargin;

 protected:

  /**
   * @brief Called when this shell frame is setup in shell view
   */
  virtual void OnSetup() = 0;

  virtual void OnResize(int width, int height) = 0;

  virtual void OnDraw(const Context *context) = 0;

  virtual int GetMouseLocation(const MouseEvent *event) const = 0;

  void EmitAction(int action) { window_action_.Emit(action); }

  AbstractShellView *shell_view() const { return shell_view_; }

 private:

  AbstractShellView *shell_view_;

  AbstractView *title_view_;

  Signal<int> window_action_;
};

}

#endif // SKLAND_ABSTRACT_WINDOW_FRAME_HPP_
