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

#ifndef SKLAND_GUI_SURFACE_HPP_
#define SKLAND_GUI_SURFACE_HPP_

#include <wayland-client.h>

#include <memory>

#include <skland/core/margin.hpp>
#include <skland/core/object.hpp>

#include <skland/wayland/client/surface.hpp>
#include <skland/wayland/client/region.hpp>

#include <skland/graphic/canvas.hpp>

namespace skland {

class Buffer;
class AbstractView;
class Region;
class Callback;
class Output;

/**
 * @brief A surface is a rectangular area that is displayed on the screen.
 *
 * A surface can have only one role at a time. Initially a
 * wl_surface does not have a role. Once a wl_surface is given a
 * role, it is set permanently for the whole lifetime of the
 * wl_surface object. Giving the current role again is allowed,
 * unless explicitly forbidden by the relevant interface
 * specification.
 */
class Surface : public Object {

  friend class AbstractView;

  Surface() = delete;
  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

 public:

  Surface(AbstractView *view, const Margin &margin = Margin());

  virtual ~Surface();

  /**
   * Create a sub-surface interface for the given surface, and
   * associate it with the given parent surface. This turns a
   * plain wl_surface into a sub-surface.
   *
   * The to-be sub-surface must not already have another role, and it
   * must not have an existing wl_subsurface object. Otherwise a protocol
   * error is raised.
   */
  void AddSubSurface(Surface *subsurface, int pos = 0);

  void Attach(const Buffer &buffer, int32_t x = 0, int32_t y = 0);

  /**
   * @brief Set input region
   * @param region
   *
   * If the parameter of region is nullptr, remove the current loaded input region.
   */
  void SetInputRegion(const wayland::client::Region &region);

  void SetOpaqueRegion(const wayland::client::Region &region);

  Surface *GetSubSurfaceAt(int index) const;

  Surface *RemoveSubSurface(Surface *subsurface);

  void Commit() const;

  void Damage(int x, int y, int width, int height) const;

  AbstractView *view() const {
    return view_;
  }

  Canvas *canvas() const {
    return canvas_sp_.get();
  }

  const Margin &margin() const {
    return margin_;
  }

  const wayland::client::Surface &wl_surface() const {
    return wl_surface_;
  }

 private:

  void OnEnter(struct wl_output *wl_output);

  void OnLeave(struct wl_output *wl_output);

  wayland::client::Surface wl_surface_;

  AbstractView *view_;

  enum wl_output_transform buffer_transform_;

  int32_t buffer_scale_;

  std::unique_ptr<Canvas> canvas_sp_;

  Margin margin_;

};

}

#endif // SKLAND_GUI_SURFACE_HPP_
