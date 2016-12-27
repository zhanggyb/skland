//
// Created by zhanggyb on 16-12-27.
//

#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>
#include <skland/gui/abstract-view.hpp>

namespace skland {

EGLSurface::EGLSurface()
    : AbstractSurface() {
}

EGLSurface::~EGLSurface() {

}

void EGLSurface::OnSetup() {
  egl_surface_.Setup(Display::egl_display(),
                     wl_surface(),
                     (int) view()->width(),
                     (int) view()->height());
}

void EGLSurface::OnAttach(const Buffer *buffer) {

}

}