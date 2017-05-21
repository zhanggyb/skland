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

#include <skland/gui/display.hpp>

#include <iostream>

#include <skland/core/exceptions.hpp>

#include <skland/gui/output.hpp>
#include <skland/gui/input.hpp>
#include <skland/gui/surface.hpp>

#include "internal/display_private.hpp"

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

  p_->egl_display.Setup(p_->wl_display);
  fprintf(stdout, "Use EGL version: %d.%d\n", p_->egl_display.major(), p_->egl_display.minor());

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

  p_->egl_display.Destroy();

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

}
