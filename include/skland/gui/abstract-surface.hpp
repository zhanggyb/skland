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

#include "../core/object.hpp"
#include "../wayland/surface.hpp"
#include "../wayland/region.hpp"
#include "task.hpp"

#include <memory>

namespace skland {

class AbstractView;

/**
 * @brief A surface represents a drawing rectangle
 *
 * A surface in 'gui' module contains a native wayland surface and
 * can provide 2D, 3D, Texture property.
 */
class AbstractSurface : public Object {

  friend class AbstractView;

  AbstractSurface() = delete;
  AbstractSurface(const AbstractSurface &) = delete;
  AbstractSurface &operator=(const AbstractSurface &) = delete;

 public:

  AbstractSurface(AbstractView *view);

  virtual ~AbstractSurface();

  void AddSubSurface(AbstractSurface *subsurface, int pos = 0);

  AbstractSurface *GetSubSurfaceAt(int index) const;

  AbstractSurface *RemoveSubSurface(AbstractSurface *subsurface);

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

  virtual std::unique_ptr<Task> CreateRenderTask() = 0;

  AbstractView *view() const {
    return view_;
  }

 protected:

  static void Attach(AbstractSurface *surface, const wayland::Buffer &wl_buffer, int32_t x = 0, int32_t y = 0) {
    surface->wl_surface_.Attach(wl_buffer, x, y);
  }

  static void Attach(AbstractSurface *surface, struct wl_buffer *wl_buffer, int32_t x = 0, int32_t y = 0) {
    surface->wl_surface_.Attach(wl_buffer, x, y);
  }

  const wayland::Surface &wl_surface() const {
    return wl_surface_;
  }

 private:

  void OnEnter(struct wl_output *wl_output);

  void OnLeave(struct wl_output *wl_output);

  wayland::Surface wl_surface_;

  AbstractView *view_;

};

}

#endif // SKLAND_ABSTRACT_SURFACE_HPP
