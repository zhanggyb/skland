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

#ifndef SKLAND_ABSTRACT_SURFACE_HPP
#define SKLAND_ABSTRACT_SURFACE_HPP

#include "task.hpp"

#include "../core/object.hpp"
#include "../core/margin.hpp"
#include "../core/point.hpp"

#include "../wayland/surface.hpp"
#include "../wayland/region.hpp"
#include "../wayland/subsurface.hpp"

#include <memory>

namespace skland {

class AbstractView;
class Canvas;
class Buffer;

/**
 * @brief A surface represents a drawing rectangle
 *
 * A surface in 'gui' module contains a native wayland surface and
 * can provide 2D, 3D, Texture property.
 */
class AbstractSurface : public Object {

  friend class AbstractView;

  AbstractSurface(const AbstractSurface &) = delete;
  AbstractSurface &operator=(const AbstractSurface &) = delete;

 public:

  AbstractSurface(const Margin &margin = Margin());

  virtual ~AbstractSurface();

  void AddSubSurface(AbstractSurface *subsurface, int pos = 0);

  AbstractSurface *RemoveSubSurface(AbstractSurface *subsurface);

  AbstractSurface *GetSubSurfaceAt(int index) const;

  void Attach(Buffer *buffer, int32_t x = 0, int32_t y = 0);

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

  const std::unique_ptr<Canvas> &canvas() const {
    return canvas_;
  }

  AbstractSurface *parent_surface() const {
    return static_cast<AbstractSurface *>(parent());
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

  void SetCanvas(Canvas *canvas);

  void set_position(int x, int y) {
    position_.x = x;
    position_.y = y;
  }

 private:

  void OnEnter(struct wl_output *wl_output);

  void OnLeave(struct wl_output *wl_output);

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

  std::unique_ptr<Canvas> canvas_;

  enum wl_output_transform buffer_transform_;

  int32_t buffer_scale_;

};

}

#endif // SKLAND_ABSTRACT_SURFACE_HPP
