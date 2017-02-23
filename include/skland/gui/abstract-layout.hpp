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

#ifndef SKLAND_GUI_ABSTRACT_LAYOUT_HPP_
#define SKLAND_GUI_ABSTRACT_LAYOUT_HPP_

#include "abstract-view.hpp"

#include "../core/margin.hpp"

namespace skland {

class AbstractLayout : public AbstractView {

  AbstractLayout(const AbstractLayout &) = delete;
  AbstractLayout &operator=(const AbstractLayout &) = delete;

 public:

  AbstractLayout(const Margin &margin = Margin(5));

  virtual ~AbstractLayout();

  virtual Size GetMinimalSize() const override;

  virtual Size GetPreferredSize() const override;

  virtual Size GetMaximalSize() const override;

  void AddView(AbstractView *view);

  const Margin &margin() const { return margin_; }

 protected:

  virtual void OnViewAdded(AbstractView *view) = 0;

  virtual void OnMouseEnter(MouseEvent *event) final;

  virtual void OnMouseLeave(MouseEvent *event) final;

  virtual void OnMouseMove(MouseEvent *event) final;

  virtual void OnMouseButton(MouseEvent *event) final;

  virtual void OnKeyboardKey(KeyEvent *event) final;

  virtual void OnDraw(const Context *context) final;

 private:

  Margin margin_;

};

}

#endif // SKLAND_GUI_ABSTRACT_LAYOUT_HPP_
