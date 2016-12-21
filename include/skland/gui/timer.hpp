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

#include <skland/core/sigcxx.hpp>
#include <skland/core/posix-timer.hpp>

namespace skland {

/**
 * @brief A timer emit signal in main thread
 */
class Timer {

 public:

  Timer();

  ~Timer();

  void Start();

  void Stop();

  void SetInterval(unsigned int interval);

  SignalRef<> timeout() {
    return timeout_;
  }

  static double GetIntervalInSeconds();

  static double GetIntervalInMilliseconds();

  static double GetIntervalInMicroseconds();

  static double GetProgramTimeInSeconds();

  static double GetProgramTimeInMilliseconds();

  static double GetProgramTimeInMicroseconds();

  static uint64_t GetMicroSeconds();

  static void SaveCurrentTime();

  static void SaveProgramTime();

  static inline uint64_t saved_time() {
    return kSavedTime;
  }

  static inline uint64_t program_time() {
    return kProgramTime;
  }

 private:

  void OnExpire();

  Signal<> timeout_;
  PosixTimer posix_timer_;

  static uint64_t kSavedTime;
  static uint64_t kProgramTime;

};

}

#endif // SKLAND_GUI_TIMER_HPP_
