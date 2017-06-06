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

#ifndef SKLAND_GUI_EGL_WINDOW_HPP_
#define SKLAND_GUI_EGL_WINDOW_HPP_

#include "abstract-shell-view.hpp"

namespace skland {

class Surface;
class Canvas;

/**
 * @ingroup gui
 * @brief A window contains an EGL surface for 3D scene
 *
 * @example simple-egl.cpp
 */
class EGLWindow : public AbstractShellView {

  EGLWindow() = delete;
  EGLWindow(const EGLWindow &) = delete;
  EGLWindow &operator=(const EGLWindow &) = delete;

 public:

  EGLWindow(const char *title);

  EGLWindow(int width, int height, const char *title);

  virtual ~EGLWindow();

 protected:

  virtual void OnShown() final;

  virtual void OnUpdate(AbstractView *view) final;

  virtual Surface *GetSurface(const AbstractView *view) const final;

  virtual bool OnConfigureSize(const Size &old_size, const Size &new_size) final;

  virtual void OnSizeChange(const Size &old_size, const Size &new_size) final;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) final;

  virtual void OnFocus(bool);

  virtual void OnInitialize();

  virtual void OnResize(int width, int height);

  virtual void OnRender();

  bool MakeCurrent();

  void SwapBuffers();

 private:

  struct Private;

  int GetMouseLocation(const MouseEvent *event) const;

  void OnFrame(uint32_t serial);

  void OnRelease();

  void RequestUpdate();

  void CancelUpdate();

  std::unique_ptr<Private> p_;

};

}

#endif // SKLAND_GUI_EGL_WINDOW_HPP_
