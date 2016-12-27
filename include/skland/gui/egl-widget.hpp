//
// Created by zhanggyb on 16-12-27.
//

#ifndef SKLAND_EGL_WIDGET_HPP
#define SKLAND_EGL_WIDGET_HPP

#include "abstract-widget.hpp"

#include "../wayland/subsurface.hpp"
#include <GLES2/gl2.h>

namespace skland {

class EGLWidget : public AbstractWidget {

 public:

  EGLWidget();

  EGLWidget(int width, int height);

  virtual ~EGLWidget();

 protected:

  virtual void OnShow() override;

  virtual void OnResize(int width, int height) override;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnDraw(Canvas *canvas) final;

 private:

  void InitializeGL ();

  GLint rotation_uniform;
  GLuint pos;
  GLuint col;

};

}

#endif //SKLAND_EGL_WIDGET_HPP
