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

#ifndef SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_
#define SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_

#include "../core/defines.hpp"
#include "../core/sigcxx.hpp"

#include <memory>

namespace skland {

class MouseEvent;
class KeyEvent;

SKLAND_EXPORT class AbstractEventHandler : public Trackable {

  friend class Input;

  friend class MouseTaskProxy;

 public:

  AbstractEventHandler();

  virtual  ~AbstractEventHandler();

 protected:

  virtual void OnMouseEnter(MouseEvent *event) = 0;

  virtual void OnMouseLeave(MouseEvent *event) = 0;

  virtual void OnMouseMove(MouseEvent *event) = 0;

  virtual void OnMouseButton(MouseEvent *event) = 0;

  virtual void OnKeyboardKey(KeyEvent *event) = 0;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

}

#endif // SKLAND_GUI_ABSTRACT_EVENT_HANDLER_HPP_
