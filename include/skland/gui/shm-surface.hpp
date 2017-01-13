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

#ifndef SKLAND_GUI_RASTER_SURFACE_HPP_
#define SKLAND_GUI_RASTER_SURFACE_HPP_

#include "abstract-surface.hpp"

#include <memory>

namespace skland {

class Buffer;

/**
 * @brief A surface using raster canvas
 */
class ShmSurface : public AbstractSurface {

  ShmSurface() = delete;
  ShmSurface(const ShmSurface &) = delete;
  ShmSurface &operator=(const ShmSurface &) = delete;

 public:

  ShmSurface(AbstractView *view, const Margin &margin = Margin());

  virtual ~ShmSurface();

  void Attach(Buffer *buffer, int32_t x = 0, int32_t y = 0);

  Canvas *GetCanvas() const final;

 private:

  std::unique_ptr<Canvas> canvas_;

};

}

#endif // SKLAND_GUI_RASTER_SURFACE_HPP_
