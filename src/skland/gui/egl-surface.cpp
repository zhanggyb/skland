//
// Created by zhanggyb on 16-12-27.
//

#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>
#include <skland/gui/abstract-view.hpp>

namespace skland {

EGLSurface::EGLSurface(AbstractView *view)
    : AbstractSurface(view) {
}

EGLSurface::~EGLSurface() {

}

void EGLSurface::OnAttach(const Buffer *buffer) {

}

}