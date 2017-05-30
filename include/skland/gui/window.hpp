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

#ifndef SKLAND_GUI_WINDOW_HPP_
#define SKLAND_GUI_WINDOW_HPP_

#include "abstract-shell-view.hpp"

namespace skland {

class AbstractWidget;
class Surface;
class Canvas;
class TitleBar;

/**
 * @ingroup gui
 * @brief A simple window with client-side decorations
 *
 * @example hello.cpp
 */
SKLAND_EXPORT class Window : public AbstractShellView {

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

 public:

  enum FlagMask {
    kFlagMaskFrameless = 0x1 << 0
  };

  Window(const char *title, int flags = 0);

  Window(int width, int height, const char *title, int flags = 0);

  virtual ~Window();

  AbstractView *GetTitleBar() const;

  AbstractView *GetContentView() const;

  void SetContentView(AbstractView *view);

 protected:

  virtual void OnShown() final;

  virtual void OnUpdate(AbstractView *view) override;

  virtual Surface *GetSurface(const AbstractView *view) const;

  virtual void OnSizeChange(const Size &old_size, const Size &new_size) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnDraw(const Context *context) override;

  virtual void OnFocus(bool);

  virtual void OnViewAttached(AbstractView *view) final;

  virtual void OnViewDetached(AbstractView *view) final;

  virtual void OnEnterOutput(const Output *output) override;

  virtual void OnLeaveOutput(const Output *output) override;

  int GetMouseLocation(const MouseEvent *event) const;

  Rect GetContentGeometry() const;

 private:

  struct Private;

  std::unique_ptr<Private> p_;

};

}

#endif // SKLAND_GUI_WINDOW_HPP_
