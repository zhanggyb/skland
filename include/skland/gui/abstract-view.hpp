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

#include "task.hpp"

#include <memory>

namespace skland {

// Forward declarations
class Application;
class Display;
class Input;
class KeyEvent;
class MouseEvent;
class TouchEvent;
class Surface;
class Context;

// Internal classes
struct ViewTask;
struct RedrawTask;
class RedrawTaskProxy;
class MouseTaskProxy;

/**
 * @ingroup gui
 * @brief An abstract base class for view object
 *
 * AbstractView is an abstract base class for view object. A view object in
 * SkLand is a rectangle area on screen and handle events. It can be a window,
 * popup menu, or widgets in a window. You typically don't use this class
 * directly. Instead, you use or create a subclass.
 *
 * A view can have parent and subviews, when you create a GUI
 * application, it generates a view hierachy.
 *
 * A view object can have arbitrary number of surfaces. A view which
 * does not have a parent must contains a shell surface. A view
 * without parent usually is a window or a popup menu.
 *
 * If a view does not have its own wayland surface (most widgets),
 * then it shares the surface with others which is managed in one of
 * parent views.
 *
 * @see AbstractWindow
 * @see Surface
 */
class AbstractView : public Object {

  friend class Application;
  friend class Display;
  friend class Input;

  friend struct RedrawTask;
  friend class RedrawTaskProxy;
  friend class MouseTaskProxy;

 public:

  /**
   * @brief Default constructor
   *
   * This create a 400 x 300 view.
   */
  AbstractView();

  /**
   * @brief Create a view by given size
   */
  AbstractView(int width, int height);

  /**
   * @brief Destructor
   */
  virtual ~AbstractView();

  void MoveTo(int x, int y);

  void Resize(int width, int height);

  int x() const { return static_cast<int>(geometry_.x()); }

  int y() const { return static_cast<int>(geometry_.y()); }

  int width() const { return static_cast<int>(geometry_.width()); }

  int height() const { return static_cast<int>(geometry_.height()); }

  float center_x() const { return geometry_.center_x(); }

  float center_y() const { return geometry_.center_y(); }

  const Rect &geometry() const { return geometry_; }

  bool visible() const { return visible_; }

  /**
   * @brief Update the display of this widget
   */
  void Update();

  virtual bool Contain(int x, int y) const;

  virtual Size GetMinimalSize() const = 0;

  virtual Size GetPreferredSize() const = 0;

  virtual Size GetMaximalSize() const = 0;

 protected:

  /**
   * @brief Update this view and all sub views
   */
  void UpdateAll();

  /**
   * @brief A view request an update
   * @param view This view or a sub view in hierachy
   */
  virtual void OnUpdate(AbstractView *view);

  /**
   * @brief Get surface for the given view
   * @param view A view object, it is always this view or a sub view in hierachy
   * @return A pointer to a surface or nullptr
   */
  virtual Surface *OnGetSurface(const AbstractView *view) const;

  virtual void OnResize(int width, int height) = 0;

  virtual void OnMouseEnter(MouseEvent *event) = 0;

  virtual void OnMouseLeave(MouseEvent *event) = 0;

  virtual void OnMouseMove(MouseEvent *event) = 0;

  virtual void OnMouseButton(MouseEvent *event) = 0;

  virtual void OnKeyboardKey(KeyEvent *event) = 0;

  virtual void OnDraw(const Context *context) = 0;

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

  /**
   * @brief Get the surface on which this view renders
   * @return A surface object or nullptr
   */
  static Surface *GetSurface(const AbstractView *view);

  static void Damage(AbstractView *view, int surface_x, int surface_y, int width, int height);

 private:

  bool visible_;

  Rect geometry_;

  /**
   * Used in event loop to render this view
   */
  std::unique_ptr<RedrawTask> redraw_task_;

  /**
   * This property used in Input to dispatch mouse events for this view
   */
  std::unique_ptr<ViewTask> mouse_task_;

  std::unique_ptr<ViewTask> mouse_motion_task_;

  // Damage area
  bool is_damaged_;
  RectI damaged_region_;

  /**
   * @brief Initialize the idle task list
   */
  static void InitializeRedrawTaskList();

  /**
   * @brief Destroy the redraw task list
   */
  static void ClearRedrawTaskList();

  static Task kRedrawTaskHead;
  static Task kRedrawTaskTail;

};

}

#endif // SKLAND_GUI_ABSTRACT_VIEW_HPP_
