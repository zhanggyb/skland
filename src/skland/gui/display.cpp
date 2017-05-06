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

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include <iostream>

#include <skland/core/exceptions.hpp>

#include <skland/gui/output.hpp>
#include <skland/gui/input.hpp>
#include <skland/gui/surface.hpp>
#include <skland/skland.hpp>

#include "internal/display_private.hpp"

using std::cout;
using std::cerr;
using std::endl;

namespace skland {

static int kEpollTaskCount = 0;

void Display::EpollTask::Run(uint32_t events) {
  ++kEpollTaskCount;
//  fprintf(stderr, "%s: %d\n", __PRETTY_FUNCTION__, kEpollTaskCount);

  display_->display_fd_events_ = events;
  if (events & EPOLLERR || events & EPOLLHUP) {
    Application::Exit();
    return;
  }
  if (events & EPOLLIN) {
    if (display_->p_->wl_display.Dispatch() == -1) {
      Application::Exit();
      return;
    }
  }
  if (events & EPOLLOUT) {
    struct epoll_event ep;
    int ret = display_->p_->wl_display.Flush();
    if (ret == 0) {
      ep.events = EPOLLIN | EPOLLERR | EPOLLHUP;
      ep.data.ptr = display_->epoll_task_;
      epoll_ctl(display_->epoll_fd_, EPOLL_CTL_MOD,
                display_->display_fd_, &ep);
    } else if (ret == -1 && errno != EAGAIN) {
      Application::Exit();
      return;
    }
  }
}

Display *Display::kDisplay = nullptr;

Display::Display()
    : epoll_fd_(0),
      epoll_task_(nullptr),
      display_fd_(0),
      display_fd_events_(0) {
  p_.reset(new Private);

  epoll_task_ = new EpollTask(this);
  cursors_.resize(kCursorBlank, nullptr);
  AbstractEventHandler::InitializeIdleTaskList();
  Surface::InitializeCommitTaskList();
}

Display::~Display() {
  delete epoll_task_;
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

  p_->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  if (p_->xkb_context == NULL) {
    throw std::runtime_error("FATAL! Cannot create xkb_context!");
  }

  epoll_fd_ = CreateEpollFd();
  display_fd_ = p_->wl_display.GetFd();
  WatchEpollFd(epoll_fd_, display_fd_, EPOLLIN | EPOLLERR | EPOLLHUP, epoll_task_);

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

  close(epoll_fd_);
  epoll_fd_ = 0;
  if (!(display_fd_events_ & EPOLLERR) &&
      !(display_fd_events_ & EPOLLHUP))
    p_->wl_display.Flush();

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

void Display::OnXdgShellPing(uint32_t serial) {
  p_->xdg_shell.Pong(serial);
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

int Display::CreateEpollFd() {
  int fd = 0;

#ifdef EPOLL_CLOEXEC
  fd = epoll_create1(EPOLL_CLOEXEC);
  if (fd >= 0)
    return fd;
  if (errno != EINVAL)
    return -1;
#endif

  fd = epoll_create(1);
  return SetCloexecOrClose(fd);
}

int Display::SetCloexecOrClose(int fd) {
  if (SetCloexec(fd) != 0) {
    close(fd);
    return -1;
  }
  return fd;
}

int Display::SetCloexec(int fd) {
  long flags;

  if (fd == -1)
    return -1;

  flags = fcntl(fd, F_GETFD);
  if (flags == -1)
    return -1;

  if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
    return -1;

  return 0;
}

void Display::WatchEpollFd(int epoll_fd, int fd, uint32_t events, void *data) {
  struct epoll_event ep;
  ep.events = events;
  ep.data.ptr = data;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ep);
}

void Display::UnwatchEpollFd(int epoll_fd, int fd) {
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
}

}
