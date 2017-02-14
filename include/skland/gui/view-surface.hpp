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

#ifndef SKLAND_GUI_VIEW_SURFACE_HPP_
#define SKLAND_GUI_VIEW_SURFACE_HPP_

#include "../core/sigcxx.hpp"
#include "../core/margin.hpp"
#include "../core/point.hpp"

#include "../wayland/surface.hpp"
#include "../wayland/region.hpp"
#include "../wayland/subsurface.hpp"

#include "task.hpp"

#include <memory>

namespace skland {

class Application;
class Display;
class AbstractView;
class SurfaceHolder;
struct CommitTaskExt;
class Buffer;

/**
 * @ingroup gui
 * @brief Surface used in views
 */
class ViewSurface {

  friend class Application;
  friend class Display;
  friend class SurfaceHolder;
  friend struct CommitTaskExt;

  ViewSurface() = delete;
  ViewSurface(const ViewSurface &) = delete;
  ViewSurface &operator=(const ViewSurface &) = delete;

 public:

  static int GetShellSurfaceCount() { return kShellSurfaceCount; }

  void Attach(Buffer *buffer, int32_t x = 0, int32_t y = 0);

  void Commit();

  void Damage(int surface_x, int surface_y, int width, int height) const {
    wl_surface_.Damage(surface_x, surface_y, width, height);
  }

  void SetInputRegion(const wayland::Region &region) {
    wl_surface_.SetInputRegion(region);
  }

  void SetOpaqueRegion(const wayland::Region &region) {
    wl_surface_.SetOpaqueRegion(region);
  }

  /**
   * @brief Get the position in window coordinates
   * @return Global position
   */
  Point GetWindowPosition() const;

  ViewSurface *parent() const { return parent_; }

  /**
   * @brief The sibling surface above this one
   * @return
   */
  ViewSurface *above() const { return above_; }

  /**
   * @brief The sibling surface below this one
   * @return
   */
  ViewSurface *below() const { return below_; }

  /**
   * @brief The shell surface shows over this one
   * @return
   */
  ViewSurface *up() const { return up_; }

  /**
   * @brief The shell surface shows under this one
   * @return
   */
  ViewSurface *down() const { return down_; }

  AbstractView *view() const { return view_; }

  const Margin &margin() const { return margin_; }

  const Point &relative_position() const { return relative_position_; }

 private:

  ViewSurface(AbstractView *view, const Margin &margin = Margin());

  virtual ~ViewSurface();

  void OnEnter(struct wl_output *wl_output);

  void OnLeave(struct wl_output *wl_output);

  /**
   * @brief The parent surface
   */
  ViewSurface *parent_;

  /**
   * @brief The sibling surface placed up
   */
  ViewSurface *above_;

  /**
   * @brief The sibling surface placed down
   */
  ViewSurface *below_;

  /**
   * @brief The shell surface shows front
   */
  ViewSurface *up_;

  /**
   * @brief The shell surface shows back
   */
  ViewSurface *down_;

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

  std::unique_ptr<CommitTaskExt> commit_task_;

  /**
   * @brief A destroyed delegate called in destructor
   */
  Signal<> destroyed_;

  size_t reference_count_;

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
  static ViewSurface *kTop;

  /**
   * @brief The bottom shell surface in the stack
   */
  static ViewSurface *kBottom;

  /**
   * @brief The count of shell surface
   */
  static int kShellSurfaceCount;

  static Task kCommitTaskHead;
  static Task kCommitTaskTail;

};

} // namespace skland

#endif // SKLAND_GUI_VIEW_SURFACE_HPP_
