//
// Created by zhanggyb on 17-2-20.
//

#ifndef SKLAND_GUI_TOOLTIP_HPP_
#define SKLAND_GUI_TOOLTIP_HPP_

#include "abstract-view.hpp"

namespace skland {

class PopupShellSurface;

class Tooltip : public AbstractView {

 public:

  Tooltip(AbstractView *view);

  virtual ~Tooltip();

  void Show();

 private:

  AbstractView *view_;  // The view object this tooltip shows up

  PopupShellSurface *popup_shell_surface_;

  std::string text_;

};

}

#endif // SKLAND_GUI_TOOLTIP_HPP_
