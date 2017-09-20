//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>

#include <skland/gui/abstract-view.hpp>
#include <skland/gui/timer.hpp>
#include <skland/gui/mouse-event.hpp>
#include <skland/gui/key-event.hpp>

#include <skland/graphic/paint.hpp>
#include <skland/graphic/canvas.hpp>

#include <skland/gui/context.hpp>

using namespace skland;
using namespace skland::gui;
using namespace skland::core;

class TimerWatcher : public Trackable {
 public:

  TimerWatcher(Timer *timer)
      : timer_(timer), count_(0) {}

  virtual ~TimerWatcher() {}

  void OnTimeout(__SLOT__) {
    fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
    count_++;
    if (count_ == 10) {
      timer_->Stop();
      Application::Exit();
    }
  }

 private:

  Timer *timer_;
  int count_;

};

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, timer_1) {
  int argc = 1;
  char argv1[] = "resize_1";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  Timer t;
  TimerWatcher watcher(&t);

  t.timeout().Connect(&watcher, &TimerWatcher::OnTimeout);
  t.SetInterval(1000000);
  t.Start();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
