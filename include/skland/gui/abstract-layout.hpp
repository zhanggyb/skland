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

#include "../core/padding.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief The base abstract layout class
 */
SKLAND_EXPORT class AbstractLayout : public AbstractView {

  AbstractLayout(const AbstractLayout &) = delete;
  AbstractLayout &operator=(const AbstractLayout &) = delete;

 public:

  AbstractLayout(const Padding &padding = Padding(5));

  void AddView(AbstractView *view);

  void RemoveView(AbstractView *view);

  const Padding &padding() const { return padding_; }

 protected:

  virtual ~AbstractLayout();

  virtual void OnChildAdded(AbstractView *view) final;

  virtual void OnChildRemoved(AbstractView *view) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

  virtual void OnViewAdded(AbstractView *view) = 0;

  virtual void OnViewRemoved(AbstractView *view) = 0;

 private:

  Padding padding_;

};

}

#endif // SKLAND_GUI_ABSTRACT_LAYOUT_HPP_
