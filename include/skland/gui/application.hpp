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

#ifndef SKLAND_GUI_APPLICATION_HPP_
#define SKLAND_GUI_APPLICATION_HPP_

#include <cstdint>

#include "display.hpp"

namespace skland {

class Display;

class Application {

 public:

  Application(int argc, char *argv[]);

  ~Application();

  Display *display() const {
    return Display::kDisplay;
  }

  static int Run();

  static void Exit();

  static inline Application *instance() {
    return Application::kInstance;
  }

 private:

  /**
   * @brief Create an epoll file descriptor
   * @return a nonnegative file descriptor or -1
   */
  int CreateEpollFd();

  /**
   * @brief Set close-on-exec or close the epoll file descriptor
   * @param fd The epoll file descriptor created in CreateEpollFd()
   * @return a nonnegative file descriptor or -1
   */
  int SetCloexecOrClose(int fd);

  /**
   * @brief Set close-on-exec flag of the given file descripor
   * @param fd The epoll file descriptor passed from CreateEpollFd()
   * @return
   *     0 - success
   *    -1 - fail
   */
  int SetCloexec(int fd);

  static void WatchEpollFd(int epoll_fd, int fd, uint32_t events, void *data);

  static void UnwatchEpollFd(int epoll_fd, int fd);

  static void HandleEpollEvents(uint32_t events);

  static void HandleSignalInt(int);

  bool running_;

  int epoll_fd_;

  static Application *kInstance;

  static const int kMaxEpollEvents = 16;

};

} // namespace skland

#endif // SKLAND_GUI_APPLICATION_HPP_
