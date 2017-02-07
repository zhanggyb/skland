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
#include "../wayland/callback.hpp"

namespace skland {

class Canvas;

/**
 * @ingroup gui
 * @brief The context used to draw a view
 */
class Context {

  Context() = delete;

 public:

  Context(AbstractSurface *surface)
      : surface_(surface) {}

  Context(const Context &other)
      : surface_(other.surface_) {}

  ~Context() {}

  Context &operator=(const Context &other) {
    surface_ = other.surface_;
    return *this;
  }

  Context &operator=(AbstractSurface *surface) {
    surface_ = surface;
    return *this;
  }

  std::shared_ptr<Canvas> GetCanvas() const {
    return surface_->GetCanvas();
  }

  void SetupCallback(wayland::Callback &callback) const {
    callback.Setup(surface_->wl_surface());
  }

  const Margin &GetMargin() const {
    return surface_->margin();
  }

  void Damage(int surface_x, int surface_y, int width, int height) const {
    surface_->Damage(surface_x, surface_y, width, height);
  }

  void Commit() const {
    surface_->Commit();
  }

  AbstractSurface *surface() const {
    return surface_;
  }

 private:

  AbstractSurface *surface_;

};

}

#endif // SKLAND_GUI_CONTEXT_HPP_
