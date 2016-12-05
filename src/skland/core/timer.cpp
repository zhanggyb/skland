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

#include <skland/core/timer.hpp>

#include <sys/time.h>
#include <errno.h>
#include <iostream>

namespace skland {

uint64_t Timer::kSavedTime = 0;

uint64_t Timer::kProgramTime = 0;

Timer::Timer(unsigned int interval)
    : id_(0),
      interval_(interval),
      is_armed_(false) {
  Create();
}

Timer::~Timer() {
  timer_delete(id_);
}

void Timer::Start() {
  int ret = -1;

  struct itimerspec ts;

  unsigned int sec = interval_ / 1000;
  long nsec = (interval_ % 1000) * 1000 * 1000;

  ts.it_interval.tv_sec = sec;
  ts.it_interval.tv_nsec = nsec;
  ts.it_value.tv_sec = sec;
  ts.it_value.tv_nsec = nsec;

  ret = timer_settime(id_, 0, &ts, 0);
  if (ret < 0) {
    std::cerr << "Fail to start timer in " << __func__ << std::endl;
    is_armed_ = false;
  } else {
    is_armed_ = true;
  }
}

void Timer::Stop() {
  if (!is_armed_) return;

  int ret = -1;
  struct itimerspec ts;
  memset(&ts, 0, sizeof(ts));

  ret = timer_settime(id_, 0, &ts, 0);
  if (ret < 0) {
    std::cerr << "Fail to stop timer in " << __func__ << std::endl;
  }

  is_armed_ = false;
}

void Timer::SetInterval(unsigned int interval) {
  if (interval_ == interval) return;
  interval_ = interval;
  if (is_armed_) Start();
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

void Timer::Create() {
  struct sigevent sev;
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_value.sival_ptr = this;
  sev.sigev_notify_function = OnExpire;
  sev.sigev_notify_attributes = 0;

  int ret = timer_create(CLOCK_REALTIME, &sev, &id_);
  if (ret < 0) {
    // do error
    std::cerr << "Fail to create timer in " << __func__ << std::endl;
    if (ret == EAGAIN) {
      std::cerr << "The calling process has already created all of the timers it is allowed by this implementation"
                << std::endl;
    }
  }
}

void Timer::OnExpire(union sigval sigev_value) {
  Timer *_this = static_cast<Timer *>(sigev_value.sival_ptr);
  if (_this->timeout_) _this->timeout_.Invoke(_this);
}

}