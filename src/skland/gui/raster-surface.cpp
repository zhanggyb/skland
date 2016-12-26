//
// Created by zhanggyb on 16-12-26.
//

#include <skland/gui/raster-surface.hpp>
#include <skland/gui/buffer.hpp>

#include "internal/redraw-task.hpp"

namespace skland {

RasterSurface::RasterSurface(AbstractView *view, const Margin &margin)
    : AbstractSurface(view), margin_(margin) {
}

RasterSurface::~RasterSurface() {

}

void RasterSurface::Attach(const Buffer &buffer, int32_t x, int32_t y) {
  if (buffer.wl_buffer().IsValid()) {
    canvas_.reset(new Canvas((unsigned char *) buffer.pixel(),
                             buffer.size().width,
                             buffer.size().height));
    canvas_->SetOrigin((float) margin_.left, (float) margin_.top);
    AbstractSurface::Attach(this, buffer.wl_buffer(), x, y);
  } else {
    canvas_.reset();
    AbstractSurface::Attach(this, NULL, x, y);
  }
}

std::unique_ptr<Task> RasterSurface::CreateRenderTask() {
  return std::unique_ptr<Task>(new RedrawTask(view(), canvas_.get()));
}

}