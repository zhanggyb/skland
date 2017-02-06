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

#ifndef SKLAND_GUI_EGL_WIDGET_HPP_
#define SKLAND_GUI_EGL_WIDGET_HPP_

#include "abstract-widget.hpp"

#include "../wayland/callback.hpp"

#include <GLES2/gl2.h>

namespace skland {

class EGLSurface;

/**
 * @brief A widget use egl surface for 3D scene
 */
class EGLWidget : public AbstractWidget {

  EGLWidget(const EGLWidget &) = delete;
  EGLWidget &operator=(const EGLWidget &) = delete;

 public:

  EGLWidget();

  EGLWidget(int width, int height);

  virtual ~EGLWidget();

 protected:

  virtual void OnUpdate(AbstractView *view) final;

  virtual AbstractSurface *OnGetSurface(const AbstractView *view) const final;

  virtual void OnResize(int width, int height) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) final;

  virtual void OnInitializeEGL();

  virtual void OnResizeEGL();

  virtual void OnRenderEGL();

 private:

  void OnFrame(uint32_t serial);

  EGLSurface *surface_;

  GLint rotation_uniform;
  GLuint pos;
  GLuint col;

  wayland::Callback frame_callback_;

  bool resize_;
  bool animating_;

};

}

#endif // SKLAND_GUI_EGL_WIDGET_HPP_
