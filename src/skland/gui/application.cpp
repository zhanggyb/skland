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

#include <skland/gui/application.hpp>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>

#include <iostream>
#include <stdlib.h>

#include <skland/core/debug.hpp>

#include <skland/gui/timer.hpp>
#include <skland/stock/theme.hpp>

#include <skland/gui/abstract-view.hpp>
#include <skland/gui/surface.hpp>

#include "internal/display_private.hpp"

/**
 * Compile-time computation of number of items in a hardcoded array.
 *
 * @param a the array being measured.
 * @return the number of items hardcoded into the array.
 */
#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH(a) (sizeof (a) / sizeof (a)[0])
#endif

using std::cerr;
using std::endl;

namespace skland {

class Application::EpollTask : public AbstractEpollTask {

 public:

  EpollTask(Application *app)
      : AbstractEpollTask(), app_(app) {}

  virtual ~EpollTask() {}

  virtual void Run(uint32_t events) override;

 private:

  Application *app_;

};

void Application::EpollTask::Run(uint32_t events) {
//  display_->display_fd_events_ = events;
  if (events & EPOLLERR || events & EPOLLHUP) {
    Application::Exit();
    return;
  }
  if (events & EPOLLIN) {
    if (Display::kDisplay->p_->wl_display.Dispatch() == -1) {
      Application::Exit();
      return;
    }
  }
  if (events & EPOLLOUT) {
    struct epoll_event ep;
    int ret = Display::kDisplay->p_->wl_display.Flush();
    if (ret == 0) {
      ep.events = EPOLLIN | EPOLLERR | EPOLLHUP;
      ep.data.ptr = app_->epoll_task_;
      epoll_ctl(app_->epoll_fd_, EPOLL_CTL_MOD,
                Display::kDisplay->display_fd_, &ep);
    } else if (ret == -1 && errno != EAGAIN) {
      Application::Exit();
      return;
    }
  }
}

Application *Application::kInstance = nullptr;

Application::Application(int argc, char *argv[])
    : running_(false), epoll_fd_(-1), epoll_task_(nullptr) {
  if (kInstance != nullptr)
    throw std::runtime_error("Error! There should be only one application instance!");

  kInstance = this;

  epoll_task_ = new EpollTask(this);

  // Set log handler to a lambda function
  wl_log_set_handler_client([](const char *format, va_list args) {
    vfprintf(stderr, format, args);
  });

  Display::kDisplay = new Display;

  try {
    Display::kDisplay->Connect(NULL);
  } catch (const std::runtime_error &e) {
    cerr << e.what() << endl;
    exit(EXIT_FAILURE);
  }

  // Load theme
  Theme::Initialize();

  epoll_fd_ = CreateEpollFd();
  WatchFd(Display::kDisplay->display_fd_, EPOLLIN | EPOLLERR | EPOLLHUP, epoll_task_);
}

Application::~Application() {
  delete epoll_task_;
  close(epoll_fd_);
  Display::kDisplay->Disconnect();

  Theme::Release();
  delete Display::kDisplay;
  Display::kDisplay = nullptr;

  kInstance = nullptr;
}

int Application::Run() {
  struct sigaction sigint;
  sigint.sa_handler = HandleSignalInt;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, NULL);

  struct epoll_event ep[kMaxEpollEvents];
  int count = 0;
  int ret = 0;

  kInstance->running_ = true;
  Task *task = nullptr;

  while (true) {
    while (AbstractView::kIdleTaskHead.next() != &AbstractView::kIdleTaskTail) {
      task = AbstractView::kIdleTaskHead.next();
      task->Unlink();
      task->Run();
    }
    while (Surface::kCommitTaskHead.next() != &Surface::kCommitTaskTail) {
      task = Surface::kCommitTaskHead.next();
      task->Unlink();
      task->Run();
    }

    Display::kDisplay->p_->wl_display.DispatchPending();

    if (!kInstance->running_) break;

    ret = Display::kDisplay->p_->wl_display.Flush();
    if (ret < 0 && errno == EAGAIN) {
      _DEBUG("%s\n", "Error when flush display");
      ep[0].events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;
      ep[0].data.ptr = kInstance->epoll_task_;
      epoll_ctl(kInstance->epoll_fd_, EPOLL_CTL_MOD,
                Display::kDisplay->display_fd_, &ep[0]);
    } else if (ret < 0) {
      break;
    }

    AbstractEpollTask *epoll_task = nullptr;
    count = epoll_wait(kInstance->epoll_fd_, ep, kMaxEpollEvents, -1);
    for (int i = 0; i < count; i++) {
      epoll_task = static_cast<AbstractEpollTask *>(ep[i].data.ptr);
      if (epoll_task) epoll_task->Run(ep[i].events);
    }
  }

  return 0;
}

void Application::Exit() {
  kInstance->running_ = false;

  // TODO: check if need to clean other resources
}

void Application::WatchFd(int fd, uint32_t events, AbstractEpollTask *epoll_task) {
  struct epoll_event ep;
  ep.events = events;
  ep.data.ptr = epoll_task;
  epoll_ctl(kInstance->epoll_fd_, EPOLL_CTL_ADD, fd, &ep);
}

void Application::UnwatchFd(int fd) {
  epoll_ctl(kInstance->epoll_fd_, EPOLL_CTL_DEL, fd, NULL);
}

void Application::HandleSignalInt(int) {
  _DEBUG("%s\n", "Get SIGINT");
  Application::Get()->Exit();
}

int Application::CreateEpollFd() {
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

int Application::SetCloexecOrClose(int fd) {
  if (SetCloexec(fd) != 0) {
    close(fd);
    return -1;
  }
  return fd;
}

int Application::SetCloexec(int fd) {
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

}
