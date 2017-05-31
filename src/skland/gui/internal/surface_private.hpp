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

#include <skland/gui/surface.hpp>

namespace skland {

struct Surface::Private {

  Private() = delete;
  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private(AbstractEventHandler *event_handler, const Margin &margin)
      : wl_surface(nullptr),
        commit_mode(kSynchronized),
        transform(kTransformNormal),
        scale(1),
        event_handler_(event_handler),
        margin(margin) {}

  ~Private() {}

  struct wl_surface *wl_surface;

  CommitMode commit_mode;
  Transform transform;
  int32_t scale;

  AbstractEventHandler *event_handler_;

  Margin margin;

  /**
   * Position in parent surface
   *
   * For root surface, this should always be (0, 0)
   */
  Point relative_position;

  static void OnEnter(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static void OnLeave(void *data, struct wl_surface *wl_surface,
                      struct wl_output *wl_output);

  static const struct wl_surface_listener kListener;

};

}

#endif // SKLAND_GUI_INTERNAL_SURFACE_PRIVATE_HPP_
