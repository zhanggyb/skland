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
#include "skland/gui/abstract-gl-interface.hpp"

namespace skland {
namespace gui {

struct Surface::Private {

  using PointI = core::PointI;

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Private);
  Private() = delete;

  Private(Surface *surface, AbstractEventHandler *event_handler, const Margin &margin)
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
        gl_interface(nullptr),
        render_task(surface),
        commit_task(surface) {
  }

  ~Private() = default;

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

  AbstractGLInterface *gl_interface;

  union {
    void *placeholder;
    Shell *shell;
    Sub *sub;
  } role;

  RenderTask render_task;
  CommitTask commit_task;

  core::Deque<AbstractView::RedrawNode> redraw_node_deque;

  static void OnEnter(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static void OnLeave(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static const struct wl_surface_listener kListener;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_INTERNAL_SURFACE_PRIVATE_HPP_
