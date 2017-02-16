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

#ifndef SKLAND_GUI_CONTEXT_HPP_
#define SKLAND_GUI_CONTEXT_HPP_

#include "abstract-surface.hpp"
#include "view-surface.hpp"

namespace skland {

class Canvas;

/**
 * @ingroup gui
 * @brief The context used to draw a view
 */
class Context {

 public:

  Context()
      : view_surface_(nullptr) {}

  Context(ViewSurface *surface, const std::shared_ptr<Canvas> canvas)
      : view_surface_(surface), canvas_(canvas) {}

  Context(const Context &other)
      : view_surface_(other.view_surface_), canvas_(other.canvas_) {}

  ~Context() {}

  Context &operator=(const Context &other) {
    view_surface_ = other.view_surface_;
    canvas_ = other.canvas_;
    return *this;
  }

  void SetupCallback(wayland::Callback &callback) const {
    view_surface_->SetupCallback(callback);
  }

  const Margin &margin() const {
    return view_surface_->margin();
  }

  void Damage(int surface_x, int surface_y, int width, int height) const {
    view_surface_->Damage(surface_x, surface_y, width, height);
  }

  void Commit() const {
    view_surface_->Commit();
  }

  ViewSurface *view_surface() const { return view_surface_; }

  const std::shared_ptr<Canvas> &canvas() const { return canvas_; }

 private:

  ViewSurface *view_surface_;
  std::shared_ptr<Canvas> canvas_;

};

}

#endif // SKLAND_GUI_CONTEXT_HPP_
