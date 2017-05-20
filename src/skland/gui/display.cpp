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
  if (p_->wl_display.IsValid()) return;

  p_->wl_display.Connect(name);
  p_->wl_display.AddListener(&Private::kDisplayListener, this);

  if (!p_->wl_display.IsValid()) {
    throw std::runtime_error("FATAL! Cannot connect to Wayland compositor!");
  }
  display_fd_ = p_->wl_display.GetFd();

  p_->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  if (p_->xkb_context == NULL) {
    throw std::runtime_error("FATAL! Cannot create xkb_context!");
  }

  p_->egl_display.Setup(p_->wl_display);
  fprintf(stdout, "Use EGL version: %d.%d\n", p_->egl_display.major(), p_->egl_display.minor());

  p_->wl_registry.Setup(p_->wl_display);
  p_->wl_registry.AddListener(&Private::kRegistryListener, this);

  if (p_->wl_display.Roundtrip() < 0) {
    Disconnect();
    throw CompositorError("Failed to process Wayland connection!");
  }

  // TODO: more operations
}

void Display::Disconnect() noexcept {
  if (!p_->wl_display.IsValid()) return;

  xkb_context_unref(p_->xkb_context);

  // TODO: other operations

  output_deque_.Clear();
  input_deque_.Clear();
//  ClearManagedObject(this, &first_window_, &last_window_, windows_count_);
  Surface::Clear();

  p_->wl_data_device_manager.Destroy();
  if (p_->wl_cursor_theme.IsValid()) {
    ReleaseCursors();
    p_->wl_cursor_theme.Destroy();
  }
  p_->wl_shell.Destroy();
  p_->xdg_shell.Destroy();
  p_->wl_shm.Destroy();
  p_->wl_subcompositor.Destroy();
  p_->wl_compositor.Destroy();
  p_->wl_registry.Destroy();

  p_->egl_display.Destroy();

  p_->wl_display.Disconnect();
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
  cursors_[kCursorBottomLeft] = Cursor::Create(p_->wl_cursor_theme.GetCursor("bottom_left_corner"));
  cursors_[kCursorBottomRight] = Cursor::Create(p_->wl_cursor_theme.GetCursor("bottom_right_corner"));
  cursors_[kCursorBottom] = Cursor::Create(p_->wl_cursor_theme.GetCursor("bottom_side"));
  cursors_[kCursorDragging] = Cursor::Create(p_->wl_cursor_theme.GetCursor("grabbing"));
  cursors_[kCursorLeftPtr] = Cursor::Create(p_->wl_cursor_theme.GetCursor("left_ptr"));
  cursors_[kCursorLeft] = Cursor::Create(p_->wl_cursor_theme.GetCursor("left_side"));
  cursors_[kCursorRight] = Cursor::Create(p_->wl_cursor_theme.GetCursor("right_side"));
  cursors_[kCursorTopLeft] = Cursor::Create(p_->wl_cursor_theme.GetCursor("top_left_corner"));
  cursors_[kCursorTopRight] = Cursor::Create(p_->wl_cursor_theme.GetCursor("top_right_corner"));
  cursors_[kCursorTop] = Cursor::Create(p_->wl_cursor_theme.GetCursor("top_side"));
  cursors_[kCursorIbeam] = Cursor::Create(p_->wl_cursor_theme.GetCursor("xterm"));
  cursors_[kCursorHand1] = Cursor::Create(p_->wl_cursor_theme.GetCursor("hand1"));
  cursors_[kCursorWatch] = Cursor::Create(p_->wl_cursor_theme.GetCursor("watch"));
  cursors_[kCursorDndMove] = Cursor::Create(p_->wl_cursor_theme.GetCursor("left_ptr"));
  cursors_[kCursorDndCopy] = Cursor::Create(p_->wl_cursor_theme.GetCursor("left_ptr"));
  cursors_[kCursorDndForbidden] = Cursor::Create(p_->wl_cursor_theme.GetCursor("left_ptr"));
}

void Display::ReleaseCursors() {
  for (size_t i = 0; i < cursors_.size(); i++) {
    delete cursors_[i];
    cursors_[i] = nullptr;
  }
}

}
