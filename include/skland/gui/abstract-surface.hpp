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

#ifndef SKLAND_ABSTRACT_SURFACE_HPP_
#define SKLAND_ABSTRACT_SURFACE_HPP_

#include "../core/object.hpp"
#include "../core/margin.hpp"
#include "../core/point.hpp"

#include "../wayland/surface.hpp"
#include "../wayland/region.hpp"
#include "../wayland/subsurface.hpp"

#include "task.hpp"

#include <memory>

namespace skland {

class Display;
class AbstractView;
class Canvas;

/**
 * @ingroup gui
 * @brief An abstract base class for surfaces
 *
 * A surface in SkLand is the drawing area on screen, it uses the native wayland
 * surface to show 2D, 3D contents.
 *
 * A surface can have a parent surface and sub surfaces. The root surface which
 * has no parent is usually used in window object and assgiend a shell surface
 * role.
 *
 * A surface can have arbitrary number of sub surfaces, use the AddSubSurface()
 * to make the given new created surface object become a sub surface placed
 * above or below the parent surface.
 *
 * According to wayland protocol, surfaces are stacked as a list, not a
 * tree. Use the above() and below() property to get the sibling surface above or
 * below current one.
 *
 * @see AbstractView
 * @see AbstractWindow
 */
class AbstractSurface {

  friend class Display;

  AbstractSurface() = delete;
  AbstractSurface(const AbstractSurface &) = delete;
  AbstractSurface &operator=(const AbstractSurface &) = delete;

 public:

  static int GetShellSurfaceCount() {
    return kCount;
  }

  AbstractSurface(AbstractView *view, const Margin &margin = Margin());

  virtual ~AbstractSurface();

  /**
   * @brief Add a new created surface into the sub surface list
   * @param subsurface A new created surface object
   * @param pos The position in the sub surface list, can be negative (below), positive (above)
   */
  void AddSubSurface(AbstractSurface *subsurface, int pos = 0);

  void SetSync() const;

  void SetDesync() const;

  void Commit() const;

  void Damage(int surface_x, int surface_y, int width, int height) const {
    wl_surface_.Damage(surface_x, surface_y, width, height);
  }

  void SetInputRegion(const wayland::Region &region) {
    wl_surface_.SetInputRegion(region);
  }

  void SetOpaqueRegion(const wayland::Region &region) {
    wl_surface_.SetOpaqueRegion(region);
  }

  void PlaceAbove(AbstractSurface *sibling);

  void PlaceBelow(AbstractSurface *sibling);

  /**
   * @brief Set sub surface position in parent surface
   * @param x X coordinate in parent surface
   * @param y Y coordinate in parent surface
   *
   * If this surface is a sub surface, set the position in parent surface.
   */
  void SetPosition(int x, int y);

  const wayland::Surface &wl_surface() const { return wl_surface_; }

  const wayland::SubSurface &wl_sub_surface() const { return wl_sub_surface_; }

  AbstractView *view() const { return view_; }

  const Margin &margin() const { return margin_; }

  const Point &position() const { return position_; }

  AbstractSurface *parent() const { return parent_; }

  /**
   * @brief The sibling surface above this one
   * @return
   */
  AbstractSurface *above() const { return above_; }

  /**
   * @brief The sibling surface below this one
   * @return
   */
  AbstractSurface *below() const { return below_; }

  /**
   * @brief The shell surface shows over this one
   * @return
   */
  AbstractSurface *up() const { return up_; }

  /**
   * @brief The shell surface shows under this one
   * @return
   */
  AbstractSurface *down() const { return down_; }

  /**
   * @brief Virtual method to get the canvas for this surface
   * @return A canvas object
   *
   * In ShmSurface this will return a pointer to a canvas object directly.
   * In EGLSurface this will render the 3D content to a canvas first.
   */
  virtual std::shared_ptr<Canvas> GetCanvas() const = 0;

 private:

  void InsertFront(AbstractSurface *surface);

  void InsertBack(AbstractSurface *surface);

  void OnEnter(struct wl_output *wl_output);

  void OnLeave(struct wl_output *wl_output);

  /**
   * @brief Move the local surface list of surface_b and insert above surface_a
   * @param surface_a
   * @param surface_b
   */
  static void MoveAbove(AbstractSurface *surface_a, AbstractSurface *surface_b);

  /**
   * @brief Move the local surface list of surface_b and insert below surface_a
   * @param surface_a
   * @param surface_b
   */
  static void MoveBelow(AbstractSurface *surface_a, AbstractSurface *surface_b);

  AbstractSurface *parent_;

  /**
   * @brief The sibling surface placed up
   */
  AbstractSurface *above_;

  /**
   * @brief The sibling surface placed down
   */
  AbstractSurface *below_;

  /**
   * @brief The shell surface shows front
   */
  AbstractSurface *up_;

  /**
   * @brief The shell surface shows back
   */
  AbstractSurface *down_;

  wayland::Surface wl_surface_;

  wayland::SubSurface wl_sub_surface_;

  AbstractView *view_;

  Margin margin_;

  /**
   * Position in parent surface
   *
   * For root surface, this should always be (0, 0)
   */
  Point position_;

  enum wl_output_transform buffer_transform_;

  int32_t buffer_scale_;

  bool is_user_data_set_;

  // global surface stack:

  /**
   * @brief Push the surface to the top of the global stack
   * @param surface
   *
   * This function is only called in constructor
   */
  static void Push(AbstractSurface *surface);

  /**
   * @brief Remove the surface from the global stack
   * @param surface
   *
   * This function is only called in destructor or when a new created surface is added into a parent
   */
  static void Remove(AbstractSurface *surface);

  /**
   * @brief Delete all shell surfaces and clear the global stack
   */
  static void Clear();

  /**
   * @brief The top shell surface in the stack
   */
  static AbstractSurface *kTop;

  /**
   * @brief The bottom shell surface in the stack
   */
  static AbstractSurface *kBottom;

  /**
   * @brief The count of shell surface
   */
  static int kCount;

};

}

#endif // SKLAND_ABSTRACT_SURFACE_HPP_
