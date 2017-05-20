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

#ifndef SKLAND_GUI_INTERNAL_DISPLAY_PRIVATE_HPP_
#define SKLAND_GUI_INTERNAL_DISPLAY_PRIVATE_HPP_

#include <skland/gui/display.hpp>

#include <skland/gui/task.hpp>
#include <skland/gui/abstract-epoll-task.hpp>

#include <skland/wayland/display.hpp>
#include <skland/wayland/registry.hpp>
#include <skland/wayland/compositor.hpp>
#include <skland/wayland/subcompositor.hpp>
#include <skland/wayland/shm.hpp>
#include <skland/wayland/shell.hpp>
#include <skland/wayland/xdg-shell.hpp>
#include <skland/wayland/cursor-theme.hpp>
#include <skland/wayland/data-device-manager.hpp>

#include <skland/egl/display.hpp>

#include "xdg-shell-unstable-v6-client-protocol.h"

namespace skland {

/**
 * @ingroup gui_intern
 * @brief The private data structure in Display
 */
struct Display::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private() {}
  ~Private() {}

  wayland::Display wl_display;
  wayland::Registry wl_registry;
  wayland::Compositor wl_compositor;
  wayland::SubCompositor wl_subcompositor;
  wayland::Shm wl_shm;
  wayland::Shell wl_shell;
  wayland::XdgShell xdg_shell;
  wayland::CursorTheme wl_cursor_theme;
  wayland::DataDeviceManager wl_data_device_manager;

  egl::Display egl_display;

  struct xkb_context *xkb_context;

  static void OnFormat(void *data, struct wl_shm *shm, uint32_t format);

  static void OnError(void *data,
                      struct wl_display *wl_display,
                      void *object_id,
                      uint32_t code,
                      const char *message);

  static void OnDeleteId(void *data,
                         struct wl_display *wl_display,
                         uint32_t id);

  static void OnGlobal(void *data,
                       struct wl_registry *registry,
                       uint32_t id,
                       const char *interface,
                       uint32_t version);

  static void OnGlobalRemove(void *data,
                             struct wl_registry *registry,
                             uint32_t name);

  static void OnPing(void *data,
                     struct zxdg_shell_v6 *zxdg_shell_v6,
                     uint32_t serial);

  static const struct wl_display_listener kDisplayListener;

  static const struct wl_registry_listener kRegistryListener;

  static const struct wl_shm_listener kShmListener;

  static const struct zxdg_shell_v6_listener kXdgShellListener;

};

}

#endif // SKLAND_GUI_INTERNAL_DISPLAY_PRIVATE_HPP_
