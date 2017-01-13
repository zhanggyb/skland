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

#include <skland/gui/shm-surface.hpp>
#include <skland/gui/buffer.hpp>

#include <skland/graphic/canvas.hpp>

namespace skland {

ShmSurface::ShmSurface(AbstractView *view, const Margin &margin)
    : AbstractSurface(view, margin) {}

ShmSurface::~ShmSurface() {
}

void ShmSurface::Attach(Buffer *buffer, int32_t x, int32_t y) {
  if (nullptr == buffer || buffer->wl_buffer().IsNull()) {
    wl_surface().Attach(NULL, x, y);
  } else {
    buffer->SetPosition(x, y);
    wl_surface().Attach(buffer->wl_buffer(), x, y);
  }

  if (nullptr == buffer) {
    canvas_.reset();
  } else {
    canvas_.reset(new Canvas((unsigned char *) buffer->pixel(),
                             buffer->size().width,
                             buffer->size().height));
    canvas_->SetOrigin((float) margin().left, (float) margin().top);
  }
}

std::shared_ptr<Canvas> ShmSurface::GetCanvas() const {
  return canvas_;
}

}
