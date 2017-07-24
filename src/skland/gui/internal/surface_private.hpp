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

#ifndef SKLAND_GUI_INTERNAL_SURFACE_PRIVATE_HPP_
#define SKLAND_GUI_INTERNAL_SURFACE_PRIVATE_HPP_

#include "skland/gui/surface.hpp"
#include "skland/gui/abstract-graphics-interface.hpp"

#include "surface_commit-task.hpp"

namespace skland {
namespace gui {

struct Surface::Private {

  using PointI = core::PointI;

  Private() = delete;
  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private(Surface *surface, AbstractEventHandler *event_handler, const core::Margin &margin)
      : wl_surface(nullptr),
        commit_mode(kSynchronized),
        transform(kTransformNormal),
        scale(1),
        event_handler(event_handler),
        margin(margin),
        parent(nullptr),
        above(nullptr),
        below(nullptr),
        upper(nullptr),
        lower(nullptr),
        egl(nullptr),
        graphics_interface(nullptr),
        commit_task(surface) {
    draw_task_head.PushBack(&draw_task_tail);
  }

  ~Private() {}

  struct wl_surface *wl_surface;

  CommitMode commit_mode;
  Transform transform;
  int32_t scale;

  AbstractEventHandler *event_handler;

  Margin margin;

  /**
   * Position in parent surface
   *
   * For root surface, this should always be (0, 0)
   */
  PointI relative_position;

  /**
    * @brief The parent surface
    */
  Surface *parent;

  /**
    * @brief The sibling surface placed up
    */
  Surface *above;

  /**
    * @brief The sibling surface placed down
    */
  Surface *below;

  /**
   * @brief The shell surface shows front
   */
  Surface *upper;

  /**
   * @brief The shell surface shows back
   */
  Surface *lower;

  EGL *egl;

  AbstractGraphicsInterface *graphics_interface;

  union {
    void *placeholder;
    Shell *shell;
    Sub *sub;
  } role;

  Task draw_task_head;
  Task draw_task_tail;

  CommitTask commit_task;

  static void OnEnter(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static void OnLeave(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static const struct wl_surface_listener kListener;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_INTERNAL_SURFACE_PRIVATE_HPP_
