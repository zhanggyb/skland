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

#include "abstract-event-handler.hpp"

#include "../core/size.hpp"
#include "../core/rect.hpp"

#include <memory>

namespace skland {

class AbstractShellView;
class Context;

/**
 * @ingroup gui
 * @brief An abstract base class for views
 *
 * AbstractView is an abstract base class for views. A view in SkLand
 * is a rectangle in an AbstractShellView object and handle events.
 * You typically don't use this class directly. Instead, you use or
 * create a subclass.
 *
 * A view can have parent and subviews, when you create a GUI
 * application, it generates a view hierachy.
 *
 * A view object can have arbitrary number of surfaces or shares the
 * surface with others which is managed in one of parent views.
 *
 * An AbstractView object SHOULD always be created by new operator and
 * MUST be destroyed by Destroy() method.
 *
 * @see AbstractShellView
 * @see Surface
 */
SKLAND_EXPORT class AbstractView : public AbstractEventHandler {

  friend class AbstractShellView;

  AbstractView(const AbstractView &) = delete;
  AbstractView &operator=(const AbstractView &) = delete;

 public:

  class Iterator;
  class ConstIterator;

  struct RedrawTask;
  class RedrawTaskIterator;

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

  void MoveTo(int x, int y);

  void Resize(int width, int height);

  int GetLeft() const;

  int GetTop() const;

  int GetRight() const;

  int GetBottom() const;

  int GetWidth() const;

  int GetHeight() const;

  float GetXCenter() const;

  float GetYCenter() const;

  const Rect &GetGeometry() const;

  bool IsVisible() const;

  /**
   * @brief Update the display of this widget
   */
  void Update();

  virtual bool Contain(int x, int y) const;

  virtual bool IsExpandX() const;

  virtual bool IsExpandY() const;

  virtual Size GetMinimalSize() const;

  virtual Size GetPreferredSize() const;

  virtual Size GetMaximalSize() const;

  /**
   * @brief Destroy and delete this object
   *
   * This method will emit an 'destroyed' signal before do some clean up work.
   */
  void Destroy();

  SignalRef<AbstractView *> destroyed() { return destroyed_; }

 protected:

  /**
   * @brief Destructor
   */
  virtual ~AbstractView();

  virtual void OnDraw(const Context *context) = 0;

  AbstractView *GetChildAt(int index) const;

  /**
   * @brief Push a child object to the front
   * @param child
   *
   * @warning This method does not check if child is nullptr
   */
  void PushFrontChild(AbstractView *child);

  /**
   * @brief Insert a child object at the given index
   * @param child
   * @param index The position to be inserted before, default is 0, same as push front
   *
   * @warning This method does not check if child is nullptr
   */
  void InsertChild(AbstractView *child, int index = 0);

  /**
   * @brief Push a child object to the back
   * @param child
   *
   * @warning This method does not check if child is nullptr
   */
  void PushBackChild(AbstractView *child);

  /**
   * @brief Remove a child object from the children list
   * @param child
   * @return nullptr if the object is not a child, or the removed object
   *
   * @warning This method does not check if the param is nullptr
   */
  AbstractView *RemoveChild(AbstractView *child);

  void ClearChildren();

  virtual void OnAddChild(AbstractView *view);

  virtual void OnRemoveChild(AbstractView *view);

  virtual void OnAddedToParent();

  virtual void OnRemovedFromParent(AbstractView *original_parent);

  virtual void OnAttachedToShellView();

  virtual void OnDetachedFromShellView(AbstractShellView *shell_view);

  static bool SwapIndex(AbstractView *object1, AbstractView *object2);

  static bool InsertSiblingBefore(AbstractView *src, AbstractView *dst);

  static bool InsertSiblingAfter(AbstractView *src, AbstractView *dst);

  static void MoveToFirst(AbstractView *object);

  static void MoveToLast(AbstractView *object);

  static void MoveForward(AbstractView *object);

  static void MoveBackward(AbstractView *object);

  /**
   * @brief Update this view and all sub views
   */
  void RecursiveUpdate();

  /**
   * @brief A view request an update
   * @param view This view or a sub view in hierachy
   */
  virtual void OnUpdate(AbstractView *view) override;

  /**
   * @brief Get surface for the given view
   * @param view A view object, it is always this view or a sub view in hierachy
   * @return A pointer to a surface or nullptr
   */
  virtual Surface *GetSurface(const AbstractView *view) const;

  virtual void OnMeasureReposition(int x, int y) = 0;

  virtual void OnMeasureResize(int width, int height) = 0;

  virtual void OnGeometryChanged(int flag, const Rect &old_geometry, const Rect &new_geometry) = 0;

  void TrackMouseMotion(MouseEvent *event);

  void UntrackMouseMotion();

  /**
   * @brief Mark damage area of the given object
   *
   * 'Damange an area in the surface' is a wayland concept.
   */
  static void Damage(AbstractView *view, int surface_x, int surface_y, int width, int height);

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  Signal<AbstractView *> destroyed_;

};

}

#endif // SKLAND_GUI_ABSTRACT_VIEW_HPP_
