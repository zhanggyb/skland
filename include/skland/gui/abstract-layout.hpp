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

namespace skland {
namespace gui {

/**
 * @ingroup gui
 * @brief The base abstract layout class
 */
SKLAND_EXPORT class AbstractLayout : public AbstractView {

  friend class AbstractView;

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractLayout);

  struct LayoutTask;

  AbstractLayout(const core::Padding &padding = core::Padding(5));

  void AddView(AbstractView *view);

  void AddView(int index, AbstractView *view);

  void AddView(int row, int column, AbstractView *view);

  void RemoveView(AbstractView *view);

  void Layout();

 protected:

  virtual ~AbstractLayout();

  virtual void OnConfigureGeometry(int dirty_flag,
                                   const core::RectF &old_geometry,
                                   const core::RectF &new_geometry) final;

  virtual void OnGeometryChange(int dirty_flag, const core::RectF &old_geometry, const core::RectF &new_geometry) final;

  virtual void OnChildAdded(AbstractView *view) final;

  virtual void OnChildRemoved(AbstractView *view) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnKeyUp(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

  virtual void OnViewAdded(AbstractView *view) = 0;

  virtual void OnViewRemoved(AbstractView *view) = 0;

 private:

  struct Private;

//  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_ABSTRACT_LAYOUT_HPP_
