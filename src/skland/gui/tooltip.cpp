//
// Created by zhanggyb on 17-2-20.
//

#include <skland/gui/tooltip.hpp>
#include <skland/gui/popup-shell-surface.hpp>

namespace skland {

Tooltip::Tooltip(AbstractView *view)
    : AbstractView(), view_(view), popup_shell_surface_(nullptr) {

}

Tooltip::~Tooltip() {

}

void Tooltip::Show() {

}

}
