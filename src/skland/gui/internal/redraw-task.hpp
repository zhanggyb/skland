//
// Created by zhanggyb on 16-12-26.
//

#ifndef SKLAND_REDRAW_TASK_HPP
#define SKLAND_REDRAW_TASK_HPP

#include <skland/gui/task.hpp>

namespace skland {

class AbstractView;
class Canvas;

struct RedrawTask : public Task {
  RedrawTask(const RedrawTask &) = delete;
  RedrawTask &operator=(const RedrawTask &) = delete;

  RedrawTask(AbstractView *view = nullptr, Canvas *canvas = nullptr)
      : Task(), view(view), canvas(canvas) {}

  virtual ~RedrawTask() {}

  virtual void Run() const final;

  AbstractView *view;

  Canvas *canvas;
};

}

#endif //SKLAND_REDRAW_TASK_HPP
