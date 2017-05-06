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

#ifndef SKLAND_GUI_DISPLAY_HPP_
#define SKLAND_GUI_DISPLAY_HPP_

#include "../core/types.hpp"
#include "../core/sigcxx.hpp"
#include "../core/deque.hpp"

#include "cursor.hpp"

#include <xkbcommon/xkbcommon.h>

#include <list>
#include <vector>
#include <set>
#include <memory>

namespace skland {

// Forward declarations

class Output;
class Input;

struct Global {
  uint32_t id;
  std::string interface;
  uint32_t version;
};

/**
 * @ingroup gui
 * @brief The global display
 */
class Display {

  friend class Application;
  friend class Output;
  friend class Input;

 public:

  class Registry;

  static const Output *GetOutputAt(int index = 0);

  static int outputs_count() {
    return kDisplay->output_deque_.count();
  }

  static bool has_pixel_format(uint32_t format) {
    return kDisplay->pixel_formats_.count(format) != 0;
  }

  static SignalRef<const Global &> unregister() {
    return kDisplay->unregister_;
  }

  static Cursor *cursor(CursorType cursor_type) {
    return kDisplay->cursors_[cursor_type];
  }

 private:

  struct Private;
  class EpollTask;

  void OnXdgShellPing(uint32_t serial);

  Display();

  virtual ~Display();

  void Connect(const char *name = NULL);

  void Disconnect() noexcept;

  void AddOutput(Output *output, int index = 0);

  void DestroyOutput(uint32_t id);

  void AddInput(Input *input, int index = 0);

  void InitializeCursors();

  void ReleaseCursors();

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

  std::unique_ptr<Private> p_;

  int epoll_fd_;
  EpollTask *epoll_task_;

  int display_fd_;
  uint32_t display_fd_events_;

  Deque output_deque_;
  Deque input_deque_;

  std::list<Global *> globals_;
  std::set<uint32_t> pixel_formats_;

  Signal<const Global &> unregister_;

  std::vector<Cursor *> cursors_;

  static Display *kDisplay;

};

} // namespace skland

#endif // SKLAND_GUI_DISPLAY_HPP_
