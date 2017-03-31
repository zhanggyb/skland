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

#ifndef SKLAND_GUI_ABSTRACT_SHELL_FRAME_HPP_
#define SKLAND_GUI_ABSTRACT_SHELL_FRAME_HPP_

#include "../core/defines.hpp"
#include "../core/sigcxx.hpp"
#include "../core/margin.hpp"
#include "../core/point.hpp"
#include "../core/rect.hpp"

#include <vector>
#include <bits/unique_ptr.h>

namespace skland {

class MouseEvent;
class AbstractView;
class AbstractShellView;
class Context;

/**
 * @ingroup gui
 * @brief The abstract class for shell view frame
 *
 * A shell frame provides decoration on a window, draw background and
 * border. It also provides a title view to display in the title bar
 * area and defines the geometry of the client view.
 */
SKLAND_EXPORT class AbstractShellFrame : public Trackable {

  friend class AbstractShellView;

  AbstractShellFrame(const AbstractShellFrame &) = delete;
  AbstractShellFrame &operator=(const AbstractShellFrame &) = delete;

 public:

  /**
   * @brief Enumeration value to indicate where to show title view
   */
  enum Position {
    kLeft,
    kTop,
    kRight,
    kBottom
  };

  AbstractShellFrame();

  virtual ~AbstractShellFrame();

  virtual Rect GetClientGeometry(int width, int height) const = 0;

  /**
   * @brief Get the view at the given position
   * @param pos
   * @return
   *   - nullptr: No view at this position
   *   - A view object: the view at this position
   *
   * @note Is reasonable to return the same view object on 2 connected sides (e.g. a button at the top-left corner)
   */
  virtual AbstractView *GetViewAt(Position pos) const = 0;

//  AbstractView *GetTitleView() const;

  SignalRef<int> action() { return action_; }

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

  void EmitAction(int action) { action_.Emit(action); }

  AbstractShellView *GetShellView() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  Signal<int> action_;
};

}

#endif // SKLAND_GUI_ABSTRACT_SHELL_FRAME_HPP_
