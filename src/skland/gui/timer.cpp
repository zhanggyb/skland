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

#include <skland/gui/abstract-epoll-task.hpp>

#include <sys/epoll.h>
#include <sys/time.h>
#include <iostream>

namespace skland {

class Timer::EpollTask : public AbstractEpollTask {

 public:

  EpollTask(Timer *timer)
      : AbstractEpollTask(), timer_(timer) {}

  virtual ~EpollTask() {}

  virtual void Run(uint32_t events) override;

 private:

  Timer *timer_;

};

void Timer::EpollTask::Run(uint32_t events) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

uint64_t Timer::kSavedTime = 0;
uint64_t Timer::kProgramTime = 0;

Timer::Timer() {
  posix_timer_.expire().Set(this, &Timer::OnExpire);
  epoll_task_.reset(new EpollTask(this));
}

Timer::~Timer() {

}

void Timer::Start() {
  posix_timer_.Start();
}

void Timer::Stop() {
  posix_timer_.Stop();
}

void Timer::SetInterval(unsigned int interval) {
  posix_timer_.SetInterval(interval);
}

void Timer::OnExpire() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
//  struct epoll_event ep[1];
//  ep[0].events = EPOLLIN | EPOLLOUT;
//  ep[0].data.ptr = epoll_task_.get();
//  epoll_ctl(Display::GetEpollFd(), EPOLL_CTL_ADD, Display::GetFd(), &ep[0]);
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
