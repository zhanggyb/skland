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

#ifndef SKLAND_GUI_ABSTRACT_VIEW_HPP_
#define SKLAND_GUI_ABSTRACT_VIEW_HPP_

#include "../core/object.hpp"
#include "../core/size.hpp"
#include "../core/rect.hpp"

#include "detail/redraw-task-node.hpp"
#include "detail/mouse-task-node.hpp"

namespace skland {

// Forward declaration
class Canvas;
class Display;
class Input;
class KeyEvent;
class MouseEvent;
class TouchEvent;
class Output;
class Application;
class Surface;

class AbstractView : public Object {

  friend class Input;
  friend class Application;
  friend class Display;
  friend class Surface;
  friend class RedrawTask;

 public:

  AbstractView();

  AbstractView(int width, int height);

  virtual ~AbstractView();

  void Show();

  void SetPosition(int x, int y);

  void Resize(int width, int height);

  virtual bool Contain(int x, int y) const;

  int x() const { return geometry_.x(); }

  int y() const { return geometry_.y(); }

  int left() const { return geometry_.left; }

  int top() const { return geometry_.top; }

  int right() const { return geometry_.right; }

  int bottom() const { return geometry_.bottom; }

  int width() const { return geometry_.width(); }

  int height() const { return geometry_.height(); }

  int center_x() const { return geometry_.center_x(); }

  int center_y() const { return geometry_.center_y(); }

  const Rect &geometry() const {
    return geometry_;
  }

  Surface *GetSurface() const;

  virtual Size GetMinimalSize() const = 0;

  virtual Size GetPreferredSize() const = 0;

  virtual Size GetMaximalSize() const = 0;

 protected:

  virtual void OnResize(int width, int height) = 0;

  virtual void OnMouseEnter(MouseEvent *event) = 0;

  virtual void OnMouseLeave(MouseEvent *event) = 0;

  virtual void OnMouseMove(MouseEvent *event) = 0;

  virtual void OnMouseButton(MouseEvent *event) = 0;

  virtual void OnKeyboardKey(KeyEvent *event) = 0;

  virtual void OnDraw(const Canvas *canvas) = 0;

  void SetSurface(Surface *surface);

  void RedrawAll();

  void set_visible(bool visible) {
    visible_ = visible;
  }

  void set_position(int x, int y) {
    geometry_.MoveTo(x, y);
  }

  void resize(int width, int height) {
    geometry_.Resize(width, height);
  }

  Surface *surface() const {
    return surface_;
  }

  AbstractView *previous_view() const {
    return static_cast<AbstractView *>(previous());
  }

  AbstractView *next_view() const {
    return static_cast<AbstractView *>(next());
  }

  AbstractView *first_subview() const {
    return static_cast<AbstractView *>(first_child());
  }

  AbstractView *last_subview() const {
    return static_cast<AbstractView *>(last_child());
  }

 private:

  static void RedrawOnSurface(AbstractView *view, Surface *surface);

  static void RedrawSubViewsOnSurface(const AbstractView *parent, Surface *surface);

  static void AddRedrawTask(RedrawTask *task);

  bool visible_;

  Rect geometry_;

  Surface *surface_;  /**< The main surface for this window */

  RedrawTask redraw_task_;
  MouseTask mouse_task_;
};

}

#endif // SKLAND_GUI_ABSTRACT_VIEW_HPP_
