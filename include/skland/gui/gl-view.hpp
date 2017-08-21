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

#ifndef SKLAND_GUI_GL_WIDGET_HPP_
#define SKLAND_GUI_GL_WIDGET_HPP_

#include "abstract-view.hpp"

#include "callback.hpp"

namespace skland {
namespace gui {

class Surface;
class AbstractRenderingAPI;

class GLView : public AbstractView {

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(GLView);

  GLView();

  void SetRenderingInterface(AbstractRenderingAPI *api);

 protected:

  virtual ~GLView();

  void OnConfigureGeometry(const RectF &old_geometry, const RectF &new_geometry) final;

  void OnSaveGeometry(const RectF &old_geometry, const RectF &new_geometry) final;

  void OnMouseEnter(MouseEvent *event) override;

  void OnMouseLeave() override;

  void OnMouseMove(MouseEvent *event) override;

  void OnMouseDown(MouseEvent *event) override;

  void OnMouseUp(MouseEvent *event) override;

  void OnKeyDown(KeyEvent *event) override;

  void OnKeyUp(KeyEvent *event) override;

  void OnDraw(const Context &context) final;

  void OnRenderSurface(Surface *surface) override;

  virtual void OnInitialize() = 0;

  virtual void OnResize(int width, int height) = 0;

  virtual void OnRender() = 0;

  void SwapBuffers();

  void MakeCurrent();

 private:

  void OnFrame(uint32_t serial);

  Surface *gl_surface_ = nullptr;

  AbstractRenderingAPI *rendering_api_ = nullptr;

  Callback callback_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_GL_WIDGET_HPP_
