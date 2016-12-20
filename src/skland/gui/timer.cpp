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

#include <skland/gui/timer.hpp>
#include <sys/time.h>

namespace skland {

uint64_t Timer::kSavedTime = 0;

uint64_t Timer::kProgramTime = 0;

Timer::Timer() {

}

Timer::~Timer() {

}

void Timer::Start() {

}

void Timer::Stop() {

}

void Timer::SetInterval(unsigned int interval) {

}

void Timer::OnTimeout(PosixTimer *posix_timer) {

}

double Timer::GetIntervalInSeconds() {
  uint64_t current = GetMicroSeconds();

  return (current - kSavedTime) / (1000000.0);
}

double Timer::GetIntervalInMilliseconds() {
  uint64_t current = GetMicroSeconds();

  return (current - kSavedTime) / (1000.0);
}

double Timer::GetIntervalInMicroseconds() {
  uint64_t current = GetMicroSeconds();

  return (double) (current - kSavedTime);
}

double Timer::GetProgramTimeInSeconds() {
  uint64_t current = GetMicroSeconds();

  return (current - kProgramTime) / (1000000.0);
}

double Timer::GetProgramTimeInMilliseconds() {
  uint64_t current = GetMicroSeconds();

  return (current - kProgramTime) / (1000.0);
}

double Timer::GetProgramTimeInMicroseconds() {
  uint64_t current = GetMicroSeconds();

  return (double) (current - kProgramTime);
}

uint64_t Timer::GetMicroSeconds() {
  uint64_t retval = 0;
  struct timeval tv = {0, 0};

  gettimeofday(&tv, NULL);
  retval = (uint64_t) tv.tv_sec * 1000 * 1000 + tv.tv_usec;

  return retval;
}

void Timer::SaveCurrentTime() {
  kSavedTime = GetMicroSeconds();
}

void Timer::SaveProgramTime() {
  kProgramTime = GetMicroSeconds();
}

} // namespace skland
