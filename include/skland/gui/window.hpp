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
namespace gui {

/**
 * @ingroup gui
 * @brief A default window with a client-side decorations
 *
 * @example hello.cpp
 */
SKLAND_EXPORT class Window : public AbstractShellView {

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Window);
  Window() = delete;

  using RectI = core::RectI;  /**< Alias of core::RectI */

  enum FlagMask {
    kFlagMaskFrameless = 0x1 << 0
  };

  /**
   * @brief Construct a 400 x 300 window with given title and flags
   * @param title
   * @param flags
   */
  Window(const char *title, int flags = 0);

  /**
   * @brief Construct a window with given size, title and flags
   * @param width
   * @param height
   * @param title
   * @param flags
   */
  Window(int width, int height, const char *title, int flags = 0);

  virtual ~Window();

  /**
   * @brief Get the title bar view
   * @return
   *    - A title bar object
   *    - nullptr if this is a frameless window
   */
  AbstractView *GetTitleBar() const;

  /**
   * @brief Get the content view
   * @return
   */
  AbstractView *GetContentView() const;

  /**
   * @brief Set the content view
   * @param view
   */
  void SetContentView(AbstractView *view);

  const Size &GetMinimalSize() const;

  const Size &GetPreferredSize() const;

  const Size &GetMaximalSize() const;

 protected:

  virtual void OnShown() final;

  virtual void OnRequestUpdate(AbstractView *view) override;

  virtual void OnConfigureSize(const Size &old_size, const Size &new_size) final;

  virtual void OnSaveSize(const Size &old_size, const Size &new_size) final;

  virtual void OnRenderSurface(Surface *surface) final;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave() override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseDown(MouseEvent *event) override;

  virtual void OnMouseUp(MouseEvent *event) override;

  virtual void OnKeyDown(KeyEvent *event) override;

  virtual void OnFocus(bool);

  virtual void OnViewAttached(AbstractView *view) final;

  virtual void OnViewDetached(AbstractView *view) final;

  virtual void OnEnterOutput(const Surface *surface, const Output *output) final;

  virtual void OnLeaveOutput(const Surface *surface, const Output *output) final;

  int GetMouseLocation(const MouseEvent *event) const;

  RectI GetContentGeometry() const;

 private:

  struct Private;

  void OnFullscreenButtonClicked(__SLOT__);

  void RenderFrame(const Context *context);

  void SetContentViewGeometry();

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_WINDOW_HPP_
