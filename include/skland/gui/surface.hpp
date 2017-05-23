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

#ifndef SKLAND_GUI_SURFACE_HPP_
#define SKLAND_GUI_SURFACE_HPP_

#include "../core/sigcxx.hpp"
#include "../core/margin.hpp"
#include "../core/point.hpp"

#include "task.hpp"

#include <wayland-egl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <memory>

namespace skland {

class Application;
class Display;
class AbstractEventHandler;
class Buffer;
class Output;
class InputEvent;
class Region;

/**
 * @ingroup gui
 * @brief Surface for views
 *
 * Surface represents a native wayland surface to display views.
 *
 * A surface can have parent or arbitrary sub surfaces and acts as different
 * roles.
 *
 * According to wayland protocol, a surface without a parent must be a shell
 * surface, a shell surface is created and managed in a AbstractShellView
 * object to display window, popup menu or tooltip etc.
 *
 * A surface which has a parent is called a sub surface, it's usually used in
 * special widgets (e.g. video widget, or EGL widget) as such widgets need their
 * own surface to display something.
 *
 * Each shell surface or sub surface can also be a EGL surface to display 3D
 * scene, otherwise, it displays 2D contents through wayland shared memory
 * buffer.
 *
 * All surfaces in an SkLand application are stacked by the order defined in
 * wayland protocol. [TODO: use an image to show the surface stack.]
 *
 * This class has several nested classes to identify and control the surface
 * role. You cannot create or delete a surface object directly. Instead, you
 * need to use the nested class. For example:
 *
 *   - Create a toplevel shell surface with Surface::Shell::Toplevel::Create()
 *   - Create a sub surface with Surface::Sub::Create()
 *
 * You can also use Surface::EGL::Get() to transform a 2D surface to 3D surface,
 * and delete the Surface::EGL object will transform back to 2D.
 */
class Surface {

  friend class Application;
  friend class Display;
  friend class Callback;

  Surface() = delete;
  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

 public:

  /**
   * @brief Shell surface role
   */
  class Shell {

    friend class Surface;

    Shell() = delete;
    Shell(const Shell &) = delete;
    Shell &operator=(const Shell &) = delete;

   public:

    /**
     * @brief Toplevel shell surface role
     */
    class Toplevel {

      friend class Shell;

      Toplevel() = delete;
      Toplevel(const Toplevel &) = delete;
      Toplevel &operator=(const Toplevel &) = delete;

     public:

      enum StatesMask {
        /**
         * the surface is maximized
         */
            kStateMaskMaximized = 0x1, /* 1 */
        /**
         * the surface is fullscreen
         */
            kStateMaskFullscreen = 0x1 << 1,  /* 2 */
        /**
         * the surface is being resized
         */
            kStateMaskResizing = 0x1 << 2, /* 4 */
        /**
         * the surface is now activated
         */
            kStateMaskActivated = 0x1 << 3, /* 8 */
      };

      /**
       * @brief Create a toplevel shell surface
       */
      static Surface *Create(AbstractEventHandler *event_handler,
                             const Margin &margin = Margin());

      static Toplevel *Get(const Surface *surface);

      void SetTitle(const char *title) const;

      void SetAppId(const char *id) const;

      void Move(const InputEvent &input_event, uint32_t serial) const;

      void Resize(const InputEvent &input_event, uint32_t serial, uint32_t edges) const;

      void SetMaximized() const;

      void UnsetMaximized() const;

      void SetFullscreen(const Output &output) const;

      void UnsetFullscreen(const Output &output) const;

      void SetMinimized() const;

     private:

      struct Private;

      Toplevel(Shell *shell_surface);

      ~Toplevel();

      std::unique_ptr<Private> p_;

      Shell *shell_;

    };

    /**
     * @brief Popup shell surface role
     */
    class Popup {

      friend class Shell;

      Popup() = delete;
      Popup(const Popup &) = delete;
      Popup &operator=(const Popup &) = delete;

     public:

      /**
       * @brief Create a popup shell surface
       */
      static Surface *Create(Shell *parent,
                             AbstractEventHandler *view,
                             const Margin &margin = Margin());

     private:

      struct Private;

      Popup(Shell *shell);

      ~Popup();

      std::unique_ptr<Private> p_;

      Shell *shell_;

    };

    static Shell *Get(const Surface *surface);

    void ResizeWindow(int width, int height) const;

    void AckConfigure(uint32_t serial) const;

    Surface *surface() const { return surface_; }

   private:

    struct Private;

    static Surface *Create(AbstractEventHandler *event_handler,
                           const Margin &margin = Margin());

    Shell(Surface *surface);

    ~Shell();

    void Push();

    void Remove();

    std::unique_ptr<Private> p_;

    Surface *surface_;

    Shell *parent_;

    union {
      void *placeholder;
      Toplevel *toplevel;
      Popup *popup;
    } role_;

  };

  /**
   * @brief Sub surface role
   */
  class Sub {

    friend class Surface;

   public:

    /**
     * @brief Create a sub surface
     */
    static Surface *Create(Surface *parent,
                           AbstractEventHandler *event_handler,
                           const Margin &margin = Margin());

    static Sub *Get(const Surface *surface);

    void PlaceAbove(Surface *sibling);

    void PlaceBelow(Surface *sibling);

    void SetRelativePosition(int x, int y);

    void SetWindowPosition(int x, int y);

    Surface *surface() const { return surface_; }

   private:

    Sub(Surface *surface, Surface *parent);

    ~Sub();

    void SetParent(Surface *parent);

    /**
   * @brief Move the local surface list and insert above target dst surface
   * @param dst
   */
    void MoveAbove(Surface *dst);

    /**
     * @brief Move the local surface list and insert below target dst surface
     * @param dst
     */
    void MoveBelow(Surface *dst);

    void InsertAbove(Surface *sibling);

    void InsertBelow(Surface *sibling);

    Surface *surface_;

    struct wl_subsurface *wl_sub_surface_;

  };

  /**
   * @brief EGL surface role
   */
  class EGL {

    EGL() = delete;
    EGL(const EGL &) = delete;
    EGL &operator=(const EGL &) = delete;

   public:

    /**
     * @brief Get the EGL surface
     *
     * If the surface is not an EGL surface, this method will create one and
     * change the surface behavior. Delete the EGL object returned by this
     * method will turn this surface back to 2D.
     */
    static EGL *Get(Surface *surface);

    virtual ~EGL();

    bool MakeCurrent();

    bool SwapBuffers();

    bool SwapBuffersWithDamage(int x, int y, int width, int height);

    bool SwapInterval(EGLint interval = 0);

    void Resize(int width, int height, int dx = 0, int dy = 0) {
      wl_egl_window_resize(wl_egl_window_, width, height, dx, dy);
    }

    Surface *surface() const { return surface_; }

   private:

    EGL(Surface *surface);

    Surface *surface_;

    EGLSurface egl_surface_;
    struct wl_egl_window *wl_egl_window_;

  };

  virtual ~Surface();

  /**
    * @brief Commit behaviour of the sub-surface
    */
  enum CommitMode {
    kSynchronized,
    kDesynchronized
  };

  static int GetShellSurfaceCount() { return kShellSurfaceCount; }

  /**
   * @brief Attach a shared memory buffer
   */
  void Attach(Buffer *buffer, int32_t x = 0, int32_t y = 0);

  /**
   * @brief Add this surface in the commit task list
   *
   * This method add this surface in the commit task list and will
   * commit the native wayland surface in the event loop.
   *
   * If this surface is a sub surface and commit behaviour is
   * synchronized, this method will commit the shell surface (main
   * surface) too.
   */
  void Commit();

  /**
   * @brief Mark the damaged region of the surface
   */
  void Damage(int surface_x, int surface_y, int width, int height) const;

  void SetInputRegion(const Region &region);

  void SetOpaqueRegion(const Region &region);

  Surface *GetShellSurface();

  /**
   * @brief Get the position in window coordinates
   * @return Global position
   */
  Point GetWindowPosition() const;

  /**
   * @brief Get the parent surface
   * @return
   */
  Surface *parent() const { return parent_; }

  /**
   * @brief The sibling surface above this one
   * @return
   */
  Surface *above() const { return above_; }

  /**
   * @brief The sibling surface below this one
   * @return
   */
  Surface *below() const { return below_; }

  /**
   * @brief The shell surface shows over this one
   * @return
   */
  Surface *up() const { return up_; }

  /**
   * @brief The shell surface shows under this one
   * @return
   */
  Surface *down() const { return down_; }

  AbstractEventHandler *event_handler() const { return event_handler_; }

  const Margin &margin() const { return margin_; }

  const Point &relative_position() const { return relative_position_; }

 private:

  struct CommitTask;

  Surface(AbstractEventHandler *event_handler, const Margin &margin = Margin());

  CommitMode mode_;

  /**
   * @brief The parent surface
   */
  Surface *parent_;

  /**
   * @brief The sibling surface placed up
   */
  Surface *above_;

  /**
   * @brief The sibling surface placed down
   */
  Surface *below_;

  /**
   * @brief The shell surface shows front
   */
  Surface *up_;

  /**
   * @brief The shell surface shows back
   */
  Surface *down_;

  struct wl_surface *wl_surface_;

  AbstractEventHandler *event_handler_;

  Margin margin_;

  /**
   * Position in parent surface
   *
   * For root surface, this should always be (0, 0)
   */
  Point relative_position_;

  enum wl_output_transform buffer_transform_;

  int32_t buffer_scale_;

  bool is_user_data_set_;

  std::unique_ptr<CommitTask> commit_task_;

  EGL *egl_;

  union {
    void *placeholder;
    Shell *shell;
    Sub *sub;
  } role_;

  // global surface stack:

  /**
   * @brief Delete all shell surfaces and clear the global stack
   */
  static void Clear();

  /**
    * @brief Initialize the idle task list
    */
  static void InitializeCommitTaskList();

  /**
   * @brief Destroy the redraw task list
   */
  static void ClearCommitTaskList();

  /**
   * @brief The top shell surface in the stack
   */
  static Surface *kTop;

  /**
   * @brief The bottom shell surface in the stack
   */
  static Surface *kBottom;

  /**
   * @brief The count of shell surface
   */
  static int kShellSurfaceCount;

  static void OnEnter(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static void OnLeave(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static const struct wl_surface_listener kListener;

  static Task kCommitTaskHead;
  static Task kCommitTaskTail;

};

} // namespace skland

#endif // SKLAND_GUI_VIEW_SURFACE_HPP_
