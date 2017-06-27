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

#ifndef SKLAND_GUI_SLIDER_BAR_HPP_
#define SKLAND_GUI_SLIDER_BAR_HPP_

#include "abstract-slider.hpp"

namespace skland {
namespace gui {

class Slider : public AbstractSlider<int> {

  Slider(const Slider &) = delete;
  Slider &operator=(const Slider &) = delete;

 public:

  Slider(Orientation orientation = kHorizontal);

  virtual ~Slider();

 protected:

  virtual void OnSizeChanged(int width, int height) final;

  virtual void OnMouseEnter(MouseEvent *event) final;

  virtual void OnMouseLeave(MouseEvent *event) final;

  virtual void OnMouseMove(MouseEvent *event) final;

  virtual void OnMouseButton(MouseEvent *event) final;

  virtual void OnKeyboardKey(KeyEvent *event) final;

  virtual void OnSetValue(const int &value) final;

  virtual void OnSetMinimum(const int &minimum) final;

  virtual void OnSetMaximum(const int &maximum) final;

  virtual void OnDraw(const Context *context) override;

 private:

  bool hover_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_SLIDER_BAR_HPP_
