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

#include "skland/gui/glesv2-interface.hpp"

#include "skland/core/assert.hpp"
#include "skland/core/memory.hpp"

#include "internal/abstract-gpu-interface_proxy.hpp"
#include "internal/display_proxy.hpp"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <wayland-egl.h>

namespace skland {
namespace gui {

struct GLESV2Interface::Private {

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);

  Private() = default;

  ~Private() = default;

  EGLSurface egl_surface = nullptr;

  struct wl_egl_window *wl_egl_window = nullptr;

};

GLESV2Interface::GLESV2Interface()
    : AbstractGPUInterface() {
  p_ = core::make_unique<Private>();
}

GLESV2Interface::~GLESV2Interface() {
  Destroy();
}

void GLESV2Interface::OnSetup() {
  Create();
}

void GLESV2Interface::Create() {
  Destroy();

  p_->wl_egl_window = wl_egl_window_create(Proxy::GetWaylandSurface(GetSurface()), 400, 300);
  p_->egl_surface = eglCreatePlatformWindowSurface(Display::Proxy::egl_display(),
                                                   Display::Proxy::egl_config(),
                                                   p_->wl_egl_window,
                                                   NULL);
}

void GLESV2Interface::Destroy() {
  if (nullptr != p_->egl_surface) {
    _ASSERT(nullptr != p_->wl_egl_window);
    eglDestroySurface(Display::Proxy::egl_display(), p_->egl_surface);
    wl_egl_window_destroy(p_->wl_egl_window);
    p_->wl_egl_window = nullptr;
    p_->egl_surface = nullptr;
  }
}

} // namespace gui
} // namespace skland
