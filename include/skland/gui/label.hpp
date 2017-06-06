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

#include <skland/core/color.hpp>

namespace skland {

// Forward declarations
class Font;

/**
 * @ingroup gui
 * @brief A label displays icon or text.
 */
SKLAND_EXPORT class Label : public AbstractView {

 public:

  Label(const std::string &text);

  Label(int width, int height, const std::string &text);

  void SetForeground(const Color &color);

  void SetBackground(const Color &color);

  void SetFont(const Font &font);

 protected:

  virtual ~Label();

  virtual void OnConfigureGeometry(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) override;

  virtual void OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) final;

  virtual void OnLayout(int dirty_flag, int left, int top, int right, int bottom) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnKeyUp(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

}

#endif // SKLAND_GUI_LABEL_HPP_
