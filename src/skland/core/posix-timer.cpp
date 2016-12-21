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
#include <skland/core/defines.hpp>

namespace skland {

PosixTimer::PosixTimer()
    : id_(0),
      interval_(0),
      is_armed_(false) {
  id_ = Create();
}

PosixTimer::~PosixTimer() {
  if (0 != id_) {
    if (0 != timer_delete(id_)) {
      DBG_PRINT_MSG("%s\n", "Not a valid POSIX timer id");
    }
  }
}

void PosixTimer::Start() {
  if (0 == id_) return;
  is_armed_ = SetTime();
}

void PosixTimer::Stop() {
  if (!is_armed_) return;

  int ret = 0;
  struct itimerspec ts;
  memset(&ts, 0, sizeof(ts));

  ret = timer_settime(id_, 0, &ts, 0);
  if (ret < 0) {
    DBG_PRINT_MSG("%s\n", "Fail to stop timer");
  }

  is_armed_ = false;
}

void PosixTimer::SetInterval(unsigned int interval) {
  if ((0 == id_) || (interval_ == interval)) return;

  interval_ = interval;

  if (is_armed_) is_armed_ = SetTime();
}

timer_t PosixTimer::Create() {
  struct sigevent sev;
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_value.sival_ptr = this;
  sev.sigev_notify_function = OnExpire;
  sev.sigev_notify_attributes = 0;

  timer_t timer = 0;

  int ret = timer_create(CLOCK_REALTIME, &sev, &timer);
  if (ret < 0) {
    DBG_PRINT_MSG("%s\n", "Fail to create timer");
    if (ret == EAGAIN) {
      DBG_PRINT_MSG("%s\n", "Temporary error during kernel allocation of timer structures.");
    } else if (ret == EINVAL) {
      DBG_PRINT_MSG("%s\n", "Clock ID, sigev_notify, sigev_signo, or sigev_notify_thread_id is invalid.");
    } else if (ret == ENOMEM) {
      DBG_PRINT_MSG("%s\n", "Could not allocate memory.");
    }
    return 0;
  }

  return timer;
}

bool PosixTimer::SetTime() {
  int ret = 0;
  struct itimerspec ts;
  unsigned int sec = interval_ / 1000;
  long nsec = (interval_ % 1000) * 1000 * 1000;

  ts.it_interval.tv_sec = sec;
  ts.it_interval.tv_nsec = nsec;
  ts.it_value.tv_sec = sec;
  ts.it_value.tv_nsec = nsec;

  ret = timer_settime(id_, 0, &ts, 0);
  if (ret < 0) {
    DBG_PRINT_MSG("%s\n", "Fail to set timer");
    return false;
  } else {
    return true;
  }
}

void PosixTimer::OnExpire(union sigval sigev_value) {
  PosixTimer *_this = static_cast<PosixTimer *>(sigev_value.sival_ptr);
  if (_this->timeout_) _this->timeout_.Invoke();
}

}
