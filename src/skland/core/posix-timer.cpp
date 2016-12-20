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

#include <skland/core/posix-timer.hpp>

#include <errno.h>
#include <iostream>

namespace skland {

PosixTimer::PosixTimer()
    : id_(0),
      interval_(0),
      is_armed_(false) {
}

PosixTimer::~PosixTimer() {
  timer_delete(id_);
}

void PosixTimer::Start() {
  if (0 == interval_) return;

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

void PosixTimer::Stop() {
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

void PosixTimer::SetInterval(unsigned int interval) {
  if ((0 == id_) && (!Create())) return;

  if (interval_ == interval) return;
  interval_ = interval;
  if (is_armed_) Start();
}

bool PosixTimer::Create() {
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
    return false;
  }

  return true;
}

void PosixTimer::OnExpire(union sigval sigev_value) {
  PosixTimer *_this = static_cast<PosixTimer *>(sigev_value.sival_ptr);
  if (_this->timeout_) _this->timeout_.Invoke(_this);
}

}
