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

namespace skland {

class AbstractView;
class Canvas;
class Buffer;

/**
 * @brief A drawing rectangle on screen
 *
 * A surface in 'gui' module contains a native wayland surface and
 * can provide 2D, 3D, Texture property.
 */
class AbstractSurface {

  friend class AbstractView;

  AbstractSurface(const AbstractSurface &) = delete;
  AbstractSurface &operator=(const AbstractSurface &) = delete;

 public:

  AbstractSurface(const Margin &margin = Margin());

  virtual ~AbstractSurface();

  /**
   * @brief Add a new created surface into the sub surface list
   * @param subsurface A new created surface object
   * @param pos The position in the sub surface list, can be negative (below), positive (above)
   */
  void AddSubSurface(AbstractSurface *subsurface, int pos = 0);

  void Attach(Buffer *buffer, int32_t x = 0, int32_t y = 0);

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
   * @brief Set sub surface position
   * @param x
   * @param y
   *
   * If this surface is a sub surface (whose wl_sub_surface_ is valid)
   */
  void SetPosition(int x, int y);

  const wayland::Surface &wl_surface() const {
    return wl_surface_;
  }

  const wayland::SubSurface &wl_sub_surface() const {
    return wl_sub_surface_;
  }

  AbstractView *view() const {
    return view_;
  }

  const Margin &margin() const {
    return margin_;
  }

  const Point &position() const {
    return position_;
  }

  AbstractSurface *parent() const {
    return parent_;
  }

  AbstractSurface *up() const {
    return up_;
  }

  AbstractSurface *down() const {
    return down_;
  }

 protected:

  /**
   * @brief Setup this surface
   *
   * This virtual method is called when a surface is set in a window.
   */
  virtual void OnSetup() = 0;

  /**
   * @brief Attach a buffer on this surface
   * @param buffer
   */
  virtual void OnAttach(const Buffer *buffer) = 0;

  void set_position(int x, int y) {
    position_.x = x;
    position_.y = y;
  }

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

  AbstractSurface *up_;

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

};

}

#endif // SKLAND_ABSTRACT_SURFACE_HPP_
