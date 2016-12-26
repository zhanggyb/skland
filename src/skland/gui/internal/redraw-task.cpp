//
// Created by zhanggyb on 16-12-26.
//

#include "redraw-task.hpp"
#include <skland/gui/abstract-view.hpp>

namespace skland {

void RedrawTask::Run() const {
  view->OnDraw(canvas);
  view->visible_ = true;

}

}
