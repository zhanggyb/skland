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

#ifndef SKLAND_GUI_ABSTRACT_WINDOW_HPP_
#define SKLAND_GUI_ABSTRACT_WINDOW_HPP_

#include "abstract-event-handler.hpp"

#include "../core/defines.hpp"
#include "../core/rect.hpp"

#include <cstdint>
#include <string>

namespace skland {

class AbstractShellFrame;

struct EventTask;

/**
 * @ingroup gui
 * @brief Abstract class to hold and manage a shell surface
 */
SKLAND_EXPORT class AbstractShellView : public AbstractEventHandler {

  friend class AbstractView;
  friend class AbstractShellFrame;

  AbstractShellView() = delete;
  AbstractShellView(const AbstractShellView &) = delete;
  AbstractShellView &operator=(const AbstractShellView &) = delete;

 public:

  enum Action {
    kActionClose,
    kActionMaximize,
    kActionMinimize,
    kActionMenu,
    kActionLast
  };

  /**
   * @brief Constructor
   * @param title A string of window title
   * @param parent Parent shell view
   * @param frame The frame used to show the background and title bar
   */
  AbstractShellView(const char *title, AbstractShellView *parent = nullptr,
                    AbstractShellFrame *frame = nullptr);

  /**
   * @brief Constructor
   * @param width Width of this shell view
   * @param height Height of this shell view
   * @param title A string of window title
   * @param parent Parent shell view
   * @param frame The frame used to show the background and title bar
   */
  AbstractShellView(int width, int height,
                    const char *title, AbstractShellView *parent = nullptr,
                    AbstractShellFrame *frame = nullptr);

  /**
   * @brief Destructor
   *
   * This destructor will destroy the content view attached and the shell frame.
   */
  virtual ~AbstractShellView();

  /**
   * @brief Set the title of this shell view
   * @param title A string of the window title
   */
  void SetTitle(const char *title);

  /**
   * @brief Set the App ID
   * @param app_id
   */
  void SetAppId(const char *app_id);

  void SetShellFrame(AbstractShellFrame *shell_frame);

  void Show();

  void Close(__SLOT__);

  void Maximize(__SLOT__);

  void Minimize(__SLOT__);

  const std::string &GetTitle() const;

  virtual Size GetMinimalSize() const;

  virtual Size GetPreferredSize() const;

  virtual Size GetMaximalSize() const;

  bool IsFullscreen() const;

  bool IsMaximized() const;

  bool IsMinimized() const;

  bool IsFocused() const;

  bool IsResizing() const;

  bool IsFrameless() const;

  bool IsShown() const;

  int GetMouseLocation(const MouseEvent *event) const;

  Rect GetClientGeometry() const;

  const Size &GetSize() const;

 protected:

  void SetClientView(AbstractView *view);

  AbstractView *GetClientView() const;

  virtual void OnShown() = 0;

  virtual void OnSizeChanged(int width, int height) = 0;

  virtual void OnMouseEnter(MouseEvent *event) override;

  virtual void OnMouseLeave(MouseEvent *event) override;

  virtual void OnMouseMove(MouseEvent *event) override;

  virtual void OnMouseButton(MouseEvent *event) override;

  virtual void OnKeyboardKey(KeyEvent *event) override;

  virtual void OnUpdate(AbstractView *view) override;

  virtual Surface *GetSurface(const AbstractView *view) const override;

  virtual void OnDraw(const Context *context) override;

  virtual void OnMaximized(bool);

  virtual void OnFullscreen(bool);

  virtual void OnFocus(bool);

  virtual void OnViewAttached(AbstractView *view);

  virtual void OnViewDetached(AbstractView *view);

  void MoveWithMouse(MouseEvent *event) const;

  void ResizeWithMouse(MouseEvent *event, uint32_t edges) const;

  AbstractShellFrame *shell_frame() const;

  Surface *shell_surface() const;

  static void ResizeShellFrame(AbstractShellFrame *window_frame, int width, int height);

  static void DrawShellFrame(AbstractShellFrame *window_frame, const Context *context);

  static void UpdateAll(AbstractView *view);

 private:

  struct Private;

  enum FlagMask {
    kFlagMaskMaximized = 0x1 << 0,
    kFlagMaskFullscreen = 0x1 << 1,
    kFlagMaskResizing = 0x1 << 2,
    kFlagMaskFocused = 0x1 << 3,
    kFlagMaskMinimized = 0x1 << 4,
    kFlagMaskShown = 0x1 << 5
  };

  void OnXdgSurfaceConfigure(uint32_t serial);

  void OnXdgToplevelConfigure(int width, int height, int states);

  void OnXdgToplevelClose();

  void OnWindowAction(int action, __SLOT__);

  void OnContentViewDestroyed(AbstractView *view, __SLOT__);

  /**
 * @brief Attach a given view to this shell view
 * @param view
 */
  void AttachView(AbstractView *view);

  /**
   * @brief Detach the view from this shell view
   * @param view
   */
  void DetachView(AbstractView *view);

  void SetContentViewGeometry();

  /**
   * @brief Dispatch mouse enter event on given parent view
   * @param parent
   * @param event
   * @param tail
   */
  void DispatchMouseEnterEvent(AbstractView *parent, MouseEvent *event, EventTask *tail);

  std::unique_ptr<Private> p_;

};

}

#endif // SKLAND_GUI_ABSTRACT_WINDOW_HPP_
