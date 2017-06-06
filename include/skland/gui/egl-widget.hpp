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

#include "abstract-view.hpp"

#include "callback.hpp"
#include "surface.hpp"

namespace skland {

/**
 * @ingroup gui
 * @brief A widget use an EGL surface for 3D scene
 */
class EGLWidget : public AbstractView {

  EGLWidget(const EGLWidget &) = delete;
  EGLWidget &operator=(const EGLWidget &) = delete;

 public:

  EGLWidget();

  EGLWidget(int width, int height);

  virtual ~EGLWidget();

 protected:

  virtual void OnUpdate(AbstractView *view) final;

  virtual Surface *GetSurface(const AbstractView *view) const final;

  virtual void OnConfigureGeometry(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) final;

  virtual void OnGeometryChange(int dirty_flag, const Rect &old_geometry, const Rect &new_geometry) final;

  virtual void OnLayout(int, int, int, int, int) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnKeyUp(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) final;

  virtual void OnInitialize();

  virtual void OnResize(int width, int height);

  virtual void OnRender();

  bool MakeCurrent();

  void SwapBuffers();

 private:

  void OnFrame(uint32_t serial);

  Surface *sub_surface_;

  Surface::EGL *egl_surface_;

  Callback frame_callback_;

  bool animating_;

};

}

#endif // SKLAND_GUI_EGL_WIDGET_HPP_
