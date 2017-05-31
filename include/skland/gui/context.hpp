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

#include "surface.hpp"
#include "callback.hpp"

namespace skland {

class Canvas;

/**
 * @ingroup gui
 * @brief The context used to draw a view
 */
class Context {

 public:

  Context()
      : surface_(nullptr) {}

  Context(Surface *surface, const std::shared_ptr<Canvas> canvas)
      : surface_(surface), canvas_(canvas) {}

  Context(const Context &other)
      : surface_(other.surface_), canvas_(other.canvas_) {}

  ~Context() {}

  Context &operator=(const Context &other) {
    surface_ = other.surface_;
    canvas_ = other.canvas_;
    return *this;
  }

  void SetupCallback(Callback &callback) const {
    callback.Setup(*surface_);
  }

  const Margin &GetMargin() const {
    return surface_->GetMargin();
  }

  void Damage(int surface_x, int surface_y, int width, int height) const {
    surface_->Damage(surface_x, surface_y, width, height);
  }

  void Commit() const {
    surface_->Commit();
  }

  Surface *surface() const { return surface_; }

  Canvas* canvas() const { return canvas_.get(); }

 private:

  Surface *surface_;
  std::shared_ptr<Canvas> canvas_;

};

}

#endif // SKLAND_GUI_CONTEXT_HPP_
