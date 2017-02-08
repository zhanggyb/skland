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

#include <skland/egl/display.hpp>

#include <EGL/eglext.h>

#include <skland/wayland/display.hpp>

#include <skland/core/defines.hpp>
#include <malloc.h>

#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH(a) (sizeof (a) / sizeof (a)[0])
#endif

namespace skland {
namespace egl {

PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC Display::kSwapBuffersWithDamageAPI = NULL;

static bool
weston_check_egl_extension(const char *extensions, const char *extension) {
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

void Display::Setup(const wayland::Display &wl_display) {
  Destroy();

  EGLint count, n, size;
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

  ret = eglInitialize(egl_display_, &major_, &minor_);
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

  static const struct {
    const char *extension, *entrypoint;
  } swap_damage_ext_to_entrypoint[] = {
      {
          .extension = "EGL_EXT_swap_buffers_with_damage",
          .entrypoint = "eglSwapBuffersWithDamageEXT",
      },
      {
          .extension = "EGL_KHR_swap_buffers_with_damage",
          .entrypoint = "eglSwapBuffersWithDamageKHR",
      },
  };
  const char *extensions;

  extensions = eglQueryString(egl_display_, EGL_EXTENSIONS);
  if (extensions &&
      weston_check_egl_extension(extensions, "EGL_EXT_buffer_age")) {
//    int len = (int) ARRAY_LENGTH(swap_damage_ext_to_entrypoint);
    int i = 0;
    for (i = 0; i < 2; i++) {
      if (weston_check_egl_extension(extensions,
                                     swap_damage_ext_to_entrypoint[i].extension)) {
        /* The EXTPROC is identical to the KHR one */
        kSwapBuffersWithDamageAPI =
            (PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC)
                eglGetProcAddress(swap_damage_ext_to_entrypoint[i].entrypoint);
        break;
      }
    }
    if (kSwapBuffersWithDamageAPI)
      printf("has EGL_EXT_buffer_age and %s\n", swap_damage_ext_to_entrypoint[i].extension);
  }
}

void Display::Destroy() {
  if (egl_display_) {
    eglMakeCurrent(egl_display_, (::EGLSurface) 0, (::EGLSurface) 0, (::EGLContext) 0);
    eglTerminate(egl_display_);
    eglReleaseThread();

    egl_display_ = nullptr;
    egl_context_ = nullptr;
    egl_config_ = nullptr;
    major_ = 0;
    minor_ = 0;
  }
}

void Display::MakeSwapBufferNonBlock() const {
  EGLint a = EGL_MIN_SWAP_INTERVAL;
  EGLint b = EGL_MAX_SWAP_INTERVAL;

  if (!eglGetConfigAttrib(egl_display_, egl_config_, a, &a) ||
      !eglGetConfigAttrib(egl_display_, egl_config_, b, &b)) {
    fprintf(stderr, "warning: swap interval range unknown\n");
  } else if (a > 0) {
    fprintf(stderr, "warning: minimum swap interval is %d, "
        "while 0 is required to not deadlock on resize.\n", a);
  }

  /*
   * We rely on the Wayland compositor to sync to vblank anyway.
   * We just need to be able to call eglSwapBuffers() without the
   * risk of waiting for a frame callback in it.
   */
  if (!eglSwapInterval(egl_display_, 0)) {
    fprintf(stderr, "error: eglSwapInterval() failed.\n");
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
