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

#include "internal/display_private.hpp"

#include <skland/core/exceptions.hpp>
#include <skland/core/assert.hpp>

#include <skland/gui/output.hpp>
#include <skland/gui/input.hpp>
#include <skland/gui/surface.hpp>

#include "internal/display_native.hpp"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

namespace skland {

Display *Display::kDisplay = nullptr;

Display::Display()
    : display_fd_(0),
      display_fd_events_(0) {
  p_.reset(new Private);

  cursors_.resize(kCursorBlank, nullptr);
  AbstractEventHandler::InitializeIdleTaskList();
  Surface::InitializeCommitTaskList();
}

Display::~Display() {
  AbstractEventHandler::ClearIdleTaskList();
  Surface::ClearCommitTaskList();
}

void Display::Connect(const char *name) {
  if (p_->wl_display) return;

  p_->wl_display = wl_display_connect(name);
  wl_display_add_listener(p_->wl_display, &Private::kDisplayListener, this);

  if (nullptr == p_->wl_display) {
    throw std::runtime_error("FATAL! Cannot connect to Wayland compositor!");
  }
  display_fd_ = wl_display_get_fd(p_->wl_display);

  p_->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  if (p_->xkb_context == NULL) {
    throw std::runtime_error("FATAL! Cannot create xkb_context!");
  }

  InitializeEGLDisplay();

  fprintf(stdout, "Use EGL version: %d.%d\n", p_->major_, p_->minor_);

  p_->wl_registry = wl_display_get_registry(p_->wl_display);
  wl_registry_add_listener(p_->wl_registry, &Private::kRegistryListener, this);

  if (wl_display_roundtrip(p_->wl_display) < 0) {
    Disconnect();
    throw CompositorError("Failed to process Wayland connection!");
  }

  // TODO: more operations
}

void Display::Disconnect() noexcept {
  if (nullptr == p_->wl_display) return;

  xkb_context_unref(p_->xkb_context);

  // TODO: other operations

  output_deque_.Clear();
  input_deque_.Clear();
  Surface::Clear();

  if (p_->wl_data_device_manager) {
    wl_data_device_manager_destroy(p_->wl_data_device_manager);
    p_->wl_data_device_manager = nullptr;
  }

  if (p_->wl_cursor_theme) {
    ReleaseCursors();
    wl_cursor_theme_destroy(p_->wl_cursor_theme);
    p_->wl_cursor_theme = nullptr;
  }

  if (p_->wl_shell) {
    wl_shell_destroy(p_->wl_shell);
    p_->wl_shell = nullptr;
  }

  if (p_->xdg_shell) {
    zxdg_shell_v6_destroy(p_->xdg_shell);
    p_->xdg_shell = nullptr;
  }

  if (p_->wl_shm) {
    wl_shm_destroy(p_->wl_shm);
    p_->wl_shm = nullptr;
  }

  if (p_->wl_subcompositor) {
    wl_subcompositor_destroy(p_->wl_subcompositor);
    p_->wl_subcompositor = nullptr;
  }

  if (p_->wl_compositor) {
    wl_compositor_destroy(p_->wl_compositor);
    p_->wl_compositor = nullptr;
  }

  if (p_->wl_registry) {
    wl_registry_destroy(p_->wl_registry);
    p_->wl_registry = nullptr;
  }

  ReleaseEGLDisplay();

  wl_display_disconnect(p_->wl_display);
}

const Output *Display::GetOutputAt(int index) {
  return static_cast<Output *>(kDisplay->output_deque_[index]);
}

void Display::AddOutput(Output *output, int index) {
  output_deque_.Insert(output, index);
}

void Display::DestroyOutput(uint32_t id) {
  Output *output = nullptr;
  for (Deque::Iterator it = output_deque_.begin(); it != output_deque_.end(); ++it) {
    output = it.cast<Output>();
    if (output->GetID() == id) {
      delete output;
      break;
    }
  }
}

void Display::AddInput(Input *input, int index) {
  input_deque_.Insert(input, index);
}

void Display::InitializeCursors() {
  cursors_[kCursorBottomLeft] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "bottom_left_corner"));
  cursors_[kCursorBottomRight] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "bottom_right_corner"));
  cursors_[kCursorBottom] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "bottom_side"));
  cursors_[kCursorDragging] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "grabbing"));
  cursors_[kCursorLeftPtr] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
  cursors_[kCursorLeft] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_side"));
  cursors_[kCursorRight] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "right_side"));
  cursors_[kCursorTopLeft] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "top_left_corner"));
  cursors_[kCursorTopRight] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "top_right_corner"));
  cursors_[kCursorTop] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "top_side"));
  cursors_[kCursorIbeam] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "xterm"));
  cursors_[kCursorHand1] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "hand1"));
  cursors_[kCursorWatch] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "watch"));
  cursors_[kCursorDndMove] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
  cursors_[kCursorDndCopy] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
  cursors_[kCursorDndForbidden] = Cursor::Create(wl_cursor_theme_get_cursor(p_->wl_cursor_theme, "left_ptr"));
}

void Display::ReleaseCursors() {
  for (size_t i = 0; i < cursors_.size(); i++) {
    delete cursors_[i];
    cursors_[i] = nullptr;
  }
}

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

void Display::InitializeEGLDisplay() {
  ReleaseEGLDisplay();

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

  p_->egl_display_ = GetEGLDisplay(EGL_PLATFORM_WAYLAND_KHR,
                                   p_->wl_display, NULL);
  _ASSERT(p_->egl_display_);

  ret = eglInitialize(p_->egl_display_, &p_->major_, &p_->minor_);
  _ASSERT(ret == EGL_TRUE);

  ret = eglBindAPI(EGL_OPENGL_ES_API);
  _ASSERT(ret == EGL_TRUE);

  eglGetConfigs(p_->egl_display_, NULL, 0, &count);

  EGLConfig *configs = (EGLConfig *) calloc((size_t) count, sizeof(EGLConfig));
  eglChooseConfig(p_->egl_display_, config_attribs, configs, count, &n);
  for (int i = 0; i < n; i++) {
    eglGetConfigAttrib(p_->egl_display_, configs[i], EGL_BUFFER_SIZE, &size);
    if (32 == size) {
      // TODO: config buffer size
      p_->egl_config_ = configs[i];
      break;
    }
  }
  free(configs);
  _ASSERT(p_->egl_config_);

  p_->egl_context_ = eglCreateContext(p_->egl_display_, p_->egl_config_, EGL_NO_CONTEXT, context_attribs);
  _ASSERT(p_->egl_context_);

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

  extensions = eglQueryString(p_->egl_display_, EGL_EXTENSIONS);
  if (extensions &&
      weston_check_egl_extension(extensions, "EGL_EXT_buffer_age")) {
//    int len = (int) ARRAY_LENGTH(swap_damage_ext_to_entrypoint);
    int i = 0;
    for (i = 0; i < 2; i++) {
      if (weston_check_egl_extension(extensions,
                                     swap_damage_ext_to_entrypoint[i].extension)) {
        /* The EXTPROC is identical to the KHR one */
        Native::kSwapBuffersWithDamageAPI =
            (PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC)
                eglGetProcAddress(swap_damage_ext_to_entrypoint[i].entrypoint);
        break;
      }
    }
    if (Native::kSwapBuffersWithDamageAPI)
      printf("has EGL_EXT_buffer_age and %s\n", swap_damage_ext_to_entrypoint[i].extension);
  }
}

void Display::ReleaseEGLDisplay() {
  if (p_->egl_display_) {
    eglMakeCurrent(p_->egl_display_, (::EGLSurface) 0, (::EGLSurface) 0, (::EGLContext) 0);
    eglTerminate(p_->egl_display_);
    eglReleaseThread();

    p_->egl_display_ = nullptr;
    p_->egl_context_ = nullptr;
    p_->egl_config_ = nullptr;
    p_->major_ = 0;
    p_->minor_ = 0;
  }
}

void Display::MakeSwapBufferNonBlock() const {
  EGLint a = EGL_MIN_SWAP_INTERVAL;
  EGLint b = EGL_MAX_SWAP_INTERVAL;

  if (!eglGetConfigAttrib(p_->egl_display_, p_->egl_config_, a, &a) ||
      !eglGetConfigAttrib(p_->egl_display_, p_->egl_config_, b, &b)) {
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
  if (!eglSwapInterval(p_->egl_display_, 0)) {
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
