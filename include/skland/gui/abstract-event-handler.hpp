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

#ifndef SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_
#define SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_

#include "../core/defines.hpp"
#include "../core/sigcxx.hpp"

#include "task.hpp"
#include <memory>

namespace skland {

class MouseEvent;
class KeyEvent;

class Surface;
class AbstractView;
class Output;

/**
 * @ingroup gui
 * @brief The base class to handle events and interact with surface
 *
 * AbstractEventHandler is the base class for most classes in GUI
 * module, it handles input events from input manager and interact with
 * the surface it hold.
 *
 * There're 2 important sub class:
 * 
 * - AbstractShellView: this is the base class for windows/menus/popups, 
 *   which have own shell surface.
 * - AbstractView: this is the base class for views layouted and show in
 *   a shell view.
 *
 * @see Input
 * @see Surface
 */
SKLAND_EXPORT class AbstractEventHandler : public Trackable {

  friend class Input;
  friend class Application;
  friend class Display;
  friend class Surface;

  friend class AbstractView;
  friend class AbstractShellView;

 public:

  struct EventTask;

  class MouseTaskIterator;

  /**
   * @brief Default constructor
   */
  AbstractEventHandler();

  /**
   * @brief Destructor
   */
  virtual ~AbstractEventHandler();

 protected:

  /**
   * @brief Virtual callback when mouse device enter this object
   */
  virtual void OnMouseEnter(MouseEvent *event) = 0;

  /**
   * @brief Virtual callback when mouse device leave this object
   */
  virtual void OnMouseLeave() = 0;

  /**
   * @brief Virtual callback when mouse moving on this object
   */
  virtual void OnMouseMove(MouseEvent *event) = 0;

  /**
   * @brief Virtual callback when mouse button pressed or released on
   * this object
   */
  virtual void OnMouseButton(MouseEvent *event) = 0;

  /**
   * @brief Virtual callback when a keyboard key pressed or released
   * on this object
   */
  virtual void OnKeyboardKey(KeyEvent *event) = 0;

  /**
   * @brief A view request an update
   * @param view A view in hierachy or null to update this object
   */
  virtual void OnUpdate(AbstractView *view) = 0;

  /**
   * @brief Get surface for the given view
   * @param view A view in hierachy or null to get shell surface for this object
   * @return A pointer to a surface or nullptr
   */
  virtual Surface *GetSurface(const AbstractView *view) const = 0;

  virtual void OnEnterOutput(const Output *output) = 0;

  virtual void OnLeaveOutput(const Output *output) = 0;

  /**
   * @brief Disable this virtual method
   * @param token
   */
  virtual void AuditDestroyingToken(details::Token */*token*/) final;

  static void PushToHead(Task *task) { kIdleTaskHead.PushBack(task); }

  static void PushToTail(Task *task) { kIdleTaskTail.PushFront(task); }

 private:

  struct Private;

  std::unique_ptr<Private> p_;

  /**
   * @brief Initialize the idle task list for redraw windows and views
   */
  static void InitializeIdleTaskList();

  /**
   * @brief Destroy the redraw task list
   */
  static void ClearIdleTaskList();

  static Task kIdleTaskHead;
  static Task kIdleTaskTail;

};

}

#endif // SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_
