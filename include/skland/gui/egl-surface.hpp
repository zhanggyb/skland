//
// Created by zhanggyb on 16-12-27.
//

#ifndef SKLAND_EGL_SURFACE_HPP
#define SKLAND_EGL_SURFACE_HPP

#include "abstract-surface.hpp"

#include "../egl/surface.hpp"

namespace skland {

class EGLSurface : public AbstractSurface {

  EGLSurface(const EGLSurface &) = delete;
  EGLSurface &operator=(const EGLSurface &) = delete;

 public:

  EGLSurface();

  virtual ~EGLSurface();

 protected:

  virtual void OnSetup() final;

  virtual void OnAttach(const Buffer *buffer) final;

 private:

  egl::Surface egl_surface_;

};

}

#endif //SKLAND_EGL_SURFACE_HPP
