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

  AbstractButton(const std::string& text);

  virtual ~AbstractButton();

  virtual Size GetPreferredSize() const override;

  SignalRef<> clicked() {
    return clicked_;
  }

  bool IsSensitive() const;

  bool IsHovered() const;

  bool IsPressed() const;

 protected:

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnMeasureReposition(int old_x, int old_y, int new_x, int new_y) override;

  virtual void OnMeasureResize(int old_width, int old_height, int new_width, int new_height) override;

  void SetSensitive(bool sensitive);

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  Signal<> clicked_;
};

}

#endif // SKLAND_GUI_ABSTRACT_BUTTON_HPP_
