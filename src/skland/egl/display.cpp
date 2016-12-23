//
// Created by zhanggyb on 16-12-23.
//

#include <skland/egl/display.hpp>

#include <EGL/eglext.h>

#include <skland/wayland/client/display.hpp>

#include <skland/core/defines.hpp>
#include <malloc.h>

namespace skland {
namespace egl {

void Display::Setup(const wayland::client::Display &wl_display) {
  Destroy();

  EGLint major, minor, count, n, size;
  EGLBoolean ret;

  EGLint config_attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_NONE
  };
  static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
  };

  egl_display_ = GetEGLDisplay(EGL_PLATFORM_WAYLAND_KHR,
                               wl_display.wl_display_, NULL);
  DBG_ASSERT(egl_display_);

  ret = eglInitialize(egl_display_, &major, &minor);
  DBG_ASSERT(ret == EGL_TRUE);

  ret = eglBindAPI(EGL_OPENGL_ES_API);
  DBG_ASSERT(ret == EGL_TRUE);

  eglGetConfigs(egl_display_, NULL, 0, &count);

  EGLConfig *configs = (EGLConfig *) calloc((size_t) count, sizeof(EGLConfig));
  eglChooseConfig(egl_display_, config_attribs, configs, count, &n);
  for (int i = 0; i < n; i++) {
    eglGetConfigAttrib(egl_display_, configs[i], EGL_BUFFER_SIZE, &size);
    if (32 == size) {
      // TODO: config buffer size
      egl_config_ = configs[i];
      break;
    }
  }
  free(configs);
  DBG_ASSERT(egl_config_);

  egl_context_ = eglCreateContext(egl_display_, egl_config_, EGL_NO_CONTEXT, context_attribs);
  DBG_ASSERT(egl_context_);
}

void Display::Destroy() {
  if (egl_display_) {
    eglTerminate(egl_display_);
    eglReleaseThread();
    egl_display_ = nullptr;
  }
}

EGLDisplay Display::GetEGLDisplay(EGLenum platform, void *native_display, const EGLint *attrib_list) {
  static PFNEGLGETPLATFORMDISPLAYEXTPROC get_platform_display = NULL;

  if (!get_platform_display) {
    get_platform_display = (PFNEGLGETPLATFORMDISPLAYEXTPROC)
        GetEGLProcAddress("eglGetPlatformDisplayEXT");
  }

  if (get_platform_display)
    return get_platform_display(platform,
                                native_display, attrib_list);

  return eglGetDisplay((EGLNativeDisplayType) native_display);
}

void *Display::GetEGLProcAddress(const char *address) {
  const char *extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

  if (extensions &&
      (CheckEGLExtension(extensions, "EGL_EXT_platform_wayland") ||
          CheckEGLExtension(extensions, "EGL_KHR_platform_wayland"))) {
    return (void *) eglGetProcAddress(address);
  }

  return NULL;
}

bool Display::CheckEGLExtension(const char *extensions, const char *extension) {
  size_t extlen = strlen(extension);
  const char *end = extensions + strlen(extensions);

  while (extensions < end) {
    size_t n = 0;

    /* Skip whitespaces, if any */
    if (*extensions == ' ') {
      extensions++;
      continue;
    }

    n = strcspn(extensions, " ");

    /* Compare strings */
    if (n == extlen && strncmp(extension, extensions, n) == 0)
      return true; /* Found */

    extensions += n;
  }

  /* Not found */
  return false;
}

}
}
