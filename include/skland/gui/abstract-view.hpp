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

#include <memory>

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
class AbstractSurface;
class AbstractWindow;

struct ViewTask;
struct RedrawTask;

/**
 * @brief A View represents an area on screen
 *
 */
class AbstractView : public Object {

  friend class Input;
  friend class Application;
  friend class Display;
  friend class AbstractSurface;
  friend struct RedrawTask;
  friend class AbstractWindow;

 public:

  AbstractView();

  AbstractView(int width, int height);

  virtual ~AbstractView();

  void SetPosition(int x, int y);

  void Resize(int width, int height);

  virtual bool Contain(int x, int y) const;

  float x() const { return geometry_.x(); }

  float y() const { return geometry_.y(); }

  float left() const { return geometry_.left; }

  float top() const { return geometry_.top; }

  float right() const { return geometry_.right; }

  float bottom() const { return geometry_.bottom; }

  float width() const { return geometry_.width(); }

  float height() const { return geometry_.height(); }

  float center_x() const { return geometry_.center_x(); }

  float center_y() const { return geometry_.center_y(); }

  const Rect &geometry() const {
    return geometry_;
  }

  AbstractSurface *GetSurface() const;

  AbstractWindow *GetWindow() const;

  /**
  * @brief Update the display of this widget
  */
  void Update();

  virtual Size GetMinimalSize() const = 0;

  virtual Size GetPreferredSize() const = 0;

  virtual Size GetMaximalSize() const = 0;

 protected:

  /**
   * @brief Update this view and all sub views
   */
  void UpdateAll();

  /**
   * @brief A sub view request an update
   * @param view
   */
  virtual void OnUpdate(AbstractView *view);

  virtual void OnResize(int width, int height) = 0;

  virtual void OnMouseEnter(MouseEvent *event) = 0;

  virtual void OnMouseLeave(MouseEvent *event) = 0;

  virtual void OnMouseMove(MouseEvent *event) = 0;

  virtual void OnMouseButton(MouseEvent *event) = 0;

  virtual void OnKeyboardKey(KeyEvent *event) = 0;

  virtual void OnDraw(Canvas *canvas) = 0;

  void SetSurface(AbstractSurface *surface);

  void TrackMouseMotion(MouseEvent *event);

  void UntrackMouseMotion();

  void set_visible(bool visible) {
    visible_ = visible;
  }

  void set_position(int x, int y) {
    geometry_.MoveTo(x, y);
  }

  void resize(int width, int height) {
    geometry_.Resize(width, height);
  }

  AbstractSurface *surface() const {
    return surface_;
  }

  AbstractView *parent_view() const {
    return static_cast<AbstractView *>(parent());
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

  const std::unique_ptr<RedrawTask> &redraw_task() const {
    return redraw_task_;
  }

  const std::unique_ptr<ViewTask> &mouse_task() const {
    return mouse_task_;
  }

 private:

  bool visible_;

  Rect geometry_;

  /**
   * The main surface for this window
   */
  AbstractSurface *surface_;

  /**
   * Used in event loop to render this view
   */
  std::unique_ptr<RedrawTask> redraw_task_;

  /**
   * This property used in Input to dispatch mouse events for this view
   */
  std::unique_ptr<ViewTask> mouse_task_;

  std::unique_ptr<ViewTask> mouse_motion_task_;

};

}

#endif // SKLAND_GUI_ABSTRACT_VIEW_HPP_
