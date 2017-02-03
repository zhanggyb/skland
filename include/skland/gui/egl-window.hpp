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

#ifndef SKLAND_EGL_WINDOW_HPP
#define SKLAND_EGL_WINDOW_HPP

#include "abstract-window.hpp"

#include "../wayland/xdg-surface.hpp"
#include "../wayland/xdg-toplevel.hpp"
#include "../wayland/region.hpp"
#include "../wayland/callback.hpp"

#include "memory-pool.hpp"
#include "buffer.hpp"

namespace skland {

class ShmSurface;
class EGLSurface;

class EGLWindow : public AbstractWindow {

  EGLWindow(const EGLWindow &) = delete;
  EGLWindow &operator=(const EGLWindow &) = delete;

 public:

  EGLWindow(const char *title, AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  EGLWindow(int width, int height, const char *title,
            AbstractWindowFrame *frame = Theme::CreateWindowFrame());

  virtual ~EGLWindow();

 protected:

  virtual void OnShown() final;

  virtual void OnInitializeEGL();

  virtual void OnResizeEGL(int width, int height);

  virtual void OnRenderEGL();

  virtual void OnUpdate(AbstractView *view) final;

  virtual AbstractSurface *OnGetSurface(const AbstractView *view) const final;

  virtual void OnResize(int width, int height) final;

 private:

  void OnFrame(uint32_t serial);

  void OnRelease();

  wayland::Region input_region_;
  wayland::Region empty_region_;

  ShmSurface *frame_surface_;
  /* Properties for frame surface, JUST experimental */
  MemoryPool frame_pool_;
  Buffer frame_buffer_;

  EGLSurface *surface_;

  wayland::Callback frame_callback_;

  bool busy_;

  bool resize_;

};

}

#endif //SKLAND_EGL_WINDOW_HPP
