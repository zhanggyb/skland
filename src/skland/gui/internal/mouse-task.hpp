//
// Created by zhanggyb on 16-12-26.
//

#ifndef SKLAND_MOUSE_TASK_HPP
#define SKLAND_MOUSE_TASK_HPP

#include <skland/gui/task.hpp>

namespace skland {

class AbstractView;

struct MouseTask : public Task {
  MouseTask(const MouseTask &) = delete;
  MouseTask &operator=(const MouseTask &) = delete;

  MouseTask(AbstractView *view = nullptr)
      : Task(), view(view) {}

  ~MouseTask() {}

  AbstractView *view;
};

}

#endif //SKLAND_MOUSE_TASK_HPP
