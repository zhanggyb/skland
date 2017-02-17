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

#include "../wayland/surface.hpp"
#include "../wayland/region.hpp"
#include "../wayland/subsurface.hpp"
#include "../wayland/callback.hpp"

#include "task.hpp"

#include <memory>

namespace skland {

class Application;
class Display;
class AbstractView;
class SurfaceHolder;
struct CommitTask;
class Buffer;

/**
 * @ingroup gui
 * @brief Surface for views
 *
 * Surface represents a native wayland surface to display views. A
 * surface object can have parent or sub surfaces. According to
 * wayland protocol, a surface without a parent must be a shell
 * surface. You can get the parent/siblings by parent(), above(),
 * below().
 *
 * A Surface can be used to display 2D contents through wayland shared
 * memory buffer, or 3D scene through EGL.
 *
 * @note You cannot create or delete a Surface object directly, it's
 * created and hold by a SurfaceHolder, which is usually used as a
 * property in ShellSurface/SubSurface/EGLSurface.
 */
class Surface {

  friend class Application;
  friend class Display;
  friend class SurfaceHolder;
  friend struct CommitTask;

  Surface() = delete;
  Surface(const Surface &) = delete;
  Surface &operator=(const Surface &) = delete;

 public:

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
  void Damage(int surface_x, int surface_y, int width, int height) const {
    wl_surface_.Damage(surface_x, surface_y, width, height);
  }

  void SetInputRegion(const wayland::Region &region) {
    wl_surface_.SetInputRegion(region);
  }

  void SetOpaqueRegion(const wayland::Region &region) {
    wl_surface_.SetOpaqueRegion(region);
  }

  void SetupCallback(wayland::Callback &callback) {
    callback.Setup(wl_surface_);
  }

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

  AbstractView *view() const { return view_; }

  const Margin &margin() const { return margin_; }

  const Point &relative_position() const { return relative_position_; }

 private:

  Surface(AbstractView *view, const Margin &margin = Margin());

  virtual ~Surface();

  void OnEnter(struct wl_output *wl_output);

  void OnLeave(struct wl_output *wl_output);

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

  wayland::Surface wl_surface_;

  AbstractView *view_;

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

  /**
   * @brief A destroyed delegate called in destructor
   */
  Signal<> destroying_;

  size_t reference_count_;

  bool egl_;

  bool is_destroying_;

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

  static Task kCommitTaskHead;
  static Task kCommitTaskTail;

};

} // namespace skland

#endif // SKLAND_GUI_VIEW_SURFACE_HPP_
