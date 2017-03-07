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

#ifndef SKLAND_GUI_LABEL_HPP_
#define SKLAND_GUI_LABEL_HPP_

#include "abstract-view.hpp"

#include "../core/color.hpp"
#include "../graphic/font.hpp"

namespace skland {

class Label : public AbstractView {

 public:

  Label(const std::string &text, const Font &font = Font());

  Label(const std::string &text, int width, int height, const Font &font = Font());

  virtual ~Label();

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

  void SetFont(const Font &font);

 protected:

  virtual void OnPositionChanged(int x, int y) override;

  virtual void OnSizeChanged(int width, int height) override;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

 private:

  std::string text_;

  Color foreground_;

  Color background_;

  Font font_;

};

}

#endif // SKLAND_GUI_LABEL_HPP_
