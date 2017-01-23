//
// Created by zhanggyb on 17-1-23.
//

#include <skland/gui/egl-window.hpp>

#include <skland/gui/egl-surface.hpp>
#include <skland/gui/display.hpp>

#include <skland/gui/key-event.hpp>
#include <skland/gui/mouse-event.hpp>

#include <skland/core/numeric.hpp>

#include <GLES2/gl2.h>

namespace skland {

EGLWindow::EGLWindow()
    : AbstractView(400, 300),
      is_xdg_surface_configured_(false),
      surface_(nullptr) {
  surface_ = new EGLSurface(this);

  input_region_.Setup(Display::wl_compositor());
  input_region_.Add(0, 0, (int) geometry().width(), (int) geometry().height());
  surface_->SetInputRegion(input_region_);

  xdg_surface_.configure().Set(this, &EGLWindow::OnXdgSurfaceConfigure);
  xdg_surface_.Setup(Display::xdg_shell(), surface_->wl_surface());

  xdg_toplevel_.configure().Set(this, &EGLWindow::OnXdgToplevelConfigure);
  xdg_toplevel_.close().Set(this, &EGLWindow::OnXdgToplevelClose);
  xdg_toplevel_.Setup(xdg_surface_);

  xdg_toplevel_.SetTitle("Test EGL surface"); // TODO: support multi-language
  xdg_toplevel_.SetAppId("Test EGL surface");
}

EGLWindow::~EGLWindow() {
  delete surface_;
}

void EGLWindow::Show() {
  if (!is_xdg_surface_configured_) {
    surface_->Commit();
  }
}

Size EGLWindow::GetMinimalSize() const {
  return Size(100, 100);
}

Size EGLWindow::GetPreferredSize() const {
  return Size(400, 300);
}

Size EGLWindow::GetMaximalSize() const {
  return Size(65536, 65536);
}

void EGLWindow::OnUpdate(AbstractView *view) {

}

AbstractSurface *EGLWindow::OnGetSurface(const AbstractView *view) const {
  return surface_;
}

void EGLWindow::OnResize(int width, int height) {
  resize(width, height);
}

void EGLWindow::OnMouseEnter(MouseEvent *event) {
  event->Accept();
}

void EGLWindow::OnMouseLeave(MouseEvent *event) {
  event->Accept();
}

void EGLWindow::OnMouseMove(MouseEvent *event) {
  event->Accept();
}

void EGLWindow::OnMouseButton(MouseEvent *event) {

}

void EGLWindow::OnKeyboardKey(KeyEvent *event) {

}

void EGLWindow::OnDraw(const Context *context) {

}

void EGLWindow::OnXdgSurfaceConfigure(uint32_t serial) {
  xdg_surface_.AckConfigure(serial);

  if (!is_xdg_surface_configured_) {
    is_xdg_surface_configured_ = true;

    xdg_surface_.SetWindowGeometry(0, 0, (int) geometry().width(), (int) geometry().height());
    surface_->Resize((int) geometry().width(), (int) geometry().height());
    surface_->Commit();
  }
}

void EGLWindow::OnXdgToplevelConfigure(int width, int height, int states) {
  using wayland::XdgToplevel;

  bool maximized = ((states & XdgToplevel::kStateMaskMaximized) != 0);
  bool fullscreen = ((states & XdgToplevel::kStateMaskFullscreen) != 0);
  bool resizing = ((states & XdgToplevel::kStateMaskResizing) != 0);
//  bool focus = ((states & XdgToplevel::kStateMaskActivated) != 0);

  Size min = this->GetMinimalSize();
  Size max = this->GetMaximalSize();
  DBG_ASSERT(min.width < max.width && min.height < max.height);

  width = clamp(width, min.width, max.width);
  height = clamp(height, min.height, max.height);

  if (maximized || fullscreen || resizing) {
    if (width != geometry().width() || height != geometry().height()) {

      input_region_.Setup(Display::wl_compositor());
      input_region_.Add(0, 0, width, height);
      surface_->SetInputRegion(input_region_);

      xdg_surface_.SetWindowGeometry(0, 0, width, height);

      resize(width, height);

      surface_->Resize((int) geometry().width(), (int) geometry().height());
      surface_->Commit();

      if (surface_->MakeCurrent()) {
        glClearColor(0.1, 0.1, .85, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        surface_->SwapBuffers();
      }

      OnResize(width, height);
    }
  }

}

void EGLWindow::OnXdgToplevelClose() {

}

}