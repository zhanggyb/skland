//
// Created by zhanggyb on 16-12-26.
//

#ifndef SKLAND_RASTER_SURFACE_HPP
#define SKLAND_RASTER_SURFACE_HPP

#include "abstract-surface.hpp"

#include "../core/margin.hpp"
#include "../graphic/canvas.hpp"

namespace skland {

class Buffer;

class RasterSurface : public AbstractSurface {

 public:

  RasterSurface(AbstractView *view, const Margin &margin = Margin());

  virtual ~RasterSurface();

  void Attach(const Buffer &buffer, int32_t x = 0, int32_t y = 0);

  virtual std::unique_ptr<Task> CreateRenderTask() final;

 private:

  std::unique_ptr<Canvas> canvas_;

  Margin margin_;

};

}

#endif //SKLAND_RASTER_SURFACE_HPP
