//
// Created by zhanggyb on 16-12-23.
//

#ifndef SKLAND_DISPLAY_HPP
#define SKLAND_DISPLAY_HPP

#include <EGL/egl.h>

namespace skland {

namespace wayland {
namespace client {
class Display;
}
}

namespace egl {

class Display {

  Display(const Display &) = delete;
  Display &operator=(const Display &) = delete;

 public:

  Display() : egl_display_(nullptr), egl_context_(nullptr), egl_config_(nullptr) {}

  ~Display() {
    if (egl_display_) {
      eglTerminate(egl_display_);
      eglReleaseThread();
    }
  }

  void Setup(const wayland::client::Display &wl_display);

  void Destroy();

 private:

  static EGLDisplay GetEGLDisplay(EGLenum platform, void *native_display, const EGLint *attrib_list);

  static void *GetEGLProcAddress(const char *address);

  static bool CheckEGLExtension(const char *extensions, const char *extension);

  EGLDisplay egl_display_;
  EGLContext egl_context_;
  EGLConfig egl_config_;

};

}
}

#endif //SKLAND_DISPLAY_HPP
