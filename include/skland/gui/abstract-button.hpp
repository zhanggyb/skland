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

#ifndef SKLAND_GUI_ABSTRACT_BUTTON_HPP_
#define SKLAND_GUI_ABSTRACT_BUTTON_HPP_

#include "abstract-view.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief The abstract class for buttons
 */
class AbstractButton : public AbstractView {

  AbstractButton(const AbstractButton &) = delete;
  AbstractButton &operator=(const AbstractButton &) = delete;

 public:

  AbstractButton();

  AbstractButton(int width, int height);

  virtual ~AbstractButton();

  virtual Size GetMinimalSize() const override;

  virtual Size GetPreferredSize() const override;

  virtual Size GetMaximalSize() const override;

  SignalRef<> clicked() {
    return clicked_;
  }

  bool IsSensitive() const {
    return (flags_ & kFlagIndexSensitive) != 0;
  }

  bool IsHovered() const {
    return (flags_ & kFlagIndexHovered) != 0;
  }

  bool IsPressed() const {
    return (flags_ & kFlagIndexPressed) != 0;
  }

 protected:

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnMeasureReposition(int x, int y) override;

  virtual void OnMeasureResize(int width, int height) override;

  virtual void OnGeometryUpdate(const Rect &new_geometry) override;

  void SetSensitive(bool sensitive);

 private:

  enum FlagIndex {

    /**! if this button is sensitive when mouse enter/leave (need to highlight when hover) */
        kFlagIndexSensitive = 0x1,

    /**! if the mouse is hovering on this button */
        kFlagIndexHovered = 0x1 << 1,

    /**! if the mouse is pressing this button */
        kFlagIndexPressed = 0x1 << 2,

    /**! if the mouse need to emit a click signal */
        kFlagIndexClicked = 0x1 << 3

  };

  uint32_t flags_;

  Signal<> clicked_;
};

}

#endif // SKLAND_GUI_ABSTRACT_BUTTON_HPP_
