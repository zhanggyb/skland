//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/timer.hpp>
#include <iostream>

using skland::Timer;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

class Response {
 public:

  inline Response (): count_(10) {}
  inline ~Response() {}

  void OnTimeout (Timer* timer);

 private:

  int count_;
};

void Response::OnTimeout(Timer* timer) {
  std::cout << "Timeout count #" << count_ << std::endl;
  count_--;
  if (count_ == 0)
    timer->Stop();
}

TEST_F(Test, timeout_1) {
  Response response;

  Timer timer;
  timer.timeout().Set(&response, &Response::OnTimeout);
  timer.SetInterval(1000);

  timer.Start();

  while(true) {
    sleep(1);
    if (!timer.is_armed()) break;
  }

  ASSERT_TRUE(true);
}
