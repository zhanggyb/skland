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

  void SetContentView(AbstractView *view);

  void Show();

  void Close(__SLOT__);

  void Maximize(__SLOT__);

  void Minimize(__SLOT__);

  const std::string &title() const { return title_; }

  virtual Size GetMinimalSize() const;

  virtual Size GetPreferredSize() const;

  virtual Size GetMaximalSize() const;

  bool IsFullscreen() const { return 0 != (flags_ & kFlagMaskFullscreen); }

  bool IsMaximized() const { return 0 != (flags_ & kFlagMaskMaximized); }

  bool IsMinimized() const { return 0 != (flags_ & kFlagMaskMinimized); }

  bool IsFocused() const { return 0 != (flags_ & kFlagMaskFocused); }

  bool IsResizing() const { return 0 != (flags_ & kFlagMaskResizing); }

  bool IsFrameless() const { return nullptr == shell_frame_; }

  bool IsShown() const { return 0 != (flags_ & kFlagMaskShown); }

  int GetMouseLocation(const MouseEvent *event) const;

  Rect GetClientGeometry() const;

  const Size &size() const { return size_; }

  AbstractView *content_view() const { return content_view_; }

 protected:

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

  virtual void OnViewDestroyed(AbstractView *view);

  virtual void OnMaximized(bool);

  virtual void OnFullscreen(bool);

  virtual void OnFocus(bool);

  virtual void OnViewAttached(AbstractView *view);

  virtual void OnViewDetached(AbstractView *view);

  void MoveWithMouse(MouseEvent *event) const;

  void ResizeWithMouse(MouseEvent *event, uint32_t edges) const;

  int flags() const { return flags_; }

  AbstractShellFrame *shell_frame() const { return shell_frame_; }

  Surface *shell_surface() const { return shell_surface_; }

  static void ResizeShellFrame(AbstractShellFrame *window_frame, int width, int height);

  static void DrawShellFrame(AbstractShellFrame *window_frame, const Context *context);

  static void UpdateAll(AbstractView *view);

 private:

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

  void SetContentViewGeometry();

  int flags_;

  Surface *shell_surface_;

  AbstractShellFrame *shell_frame_;

  std::string title_;
  std::string app_id_;

  Size size_;

  AbstractView *content_view_;

  AbstractShellView *parent_;

};

}

#endif // SKLAND_GUI_ABSTRACT_WINDOW_HPP_
