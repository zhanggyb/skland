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

#ifndef SKLAND_GUI_TIMER_HPP_
#define SKLAND_GUI_TIMER_HPP_

#include "../core/sigcxx.hpp"

#include <memory>

namespace skland {

/**
 * @brief A timer emit signal in main thread
 */
class Timer {

 public:

  /**
   * @brief Default constructor
   * @param interval Interval in microseconds
   */
  Timer(unsigned int interval = 5000000);

  ~Timer();

  void Start();

  void Stop();

  void SetInterval(unsigned int interval);

  unsigned int GetInterval() const;

  bool IsArmed() const;

  SignalRef<> timeout() { return timeout_; }

  /**
   * @brief Get clock time in nanoseconds
   * @return
   */
  static uint64_t GetClockTime();

 private:

  struct Private;
  class EpollTask;

  bool SetTime();

  std::unique_ptr<Private> p_;

  Signal<> timeout_;

};

}

#endif // SKLAND_GUI_TIMER_HPP_
