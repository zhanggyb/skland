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

#ifndef SKLAND_GUI_DISPLAY_HPP_
#define SKLAND_GUI_DISPLAY_HPP_

#include "cursor.hpp"

#include "../core/object.hpp"
#include "../core/types.hpp"

#include "../egl/display.hpp"

#include "task.hpp"

//#include <xkbcommon/xkbcommon.h>

#include <list>
#include <set>
#include <vector>

#include <skland/wayland/display.hpp>
#include <skland/wayland/registry.hpp>
#include <skland/wayland/compositor.hpp>
#include <skland/wayland/subcompositor.hpp>
#include <skland/wayland/shm.hpp>
#include <skland/wayland/shell.hpp>
#include <skland/wayland/xdg-shell.hpp>
#include <skland/wayland/cursor-theme.hpp>
#include <skland/wayland/data-device-manager.hpp>

#include <EGL/egl.h>

namespace skland {

// Forward declarations

class AbstractView;
class AbstractWindow;
class Output;
class Input;

struct Global {
  uint32_t id;
  std::string interface;
  uint32_t version;
};

class Display : public Object {

  friend class AbstractView;
  friend class AbstractWindow;
  friend class Application;
  friend class Output;
  friend class Input;

 public:

  static void AddWindow(AbstractWindow *window, int pos = 0);

  static const Output *GetOutputAt(int index = 0);

  static AbstractWindow *GetWindowAt(int index = 0);

  static int windows_count() {
    return kDisplay->windows_count_;
  }

  static int outputs_count() {
    return kDisplay->outputs_count_;
  }

  static bool has_pixel_format(uint32_t format) {
    return kDisplay->pixel_formats_.count(format) != 0;
  }

  static SignalRef<const Global &> unregister() {
    return kDisplay->unregister_;
  }

  static Cursor *cursor(CursorType cursor_type) {
    return kDisplay->cursors_[cursor_type];
  }

  static const wayland::Display &wl_display() {
    return kDisplay->wl_display_;
  }

  static const wayland::Compositor &wl_compositor() {
    return kDisplay->wl_compositor_;
  }

  static const wayland::SubCompositor &wl_subcompositor() {
    return kDisplay->wl_subcompositor_;
  }

  static const wayland::Shm &wl_shm() {
    return kDisplay->wl_shm_;
  }

  static const wayland::XdgShell &xdg_shell() {
    return kDisplay->xdg_shell_;
  }

  static const wayland::Shell &wl_shell() {
    return kDisplay->wl_shell_;
  }

  static const wayland::DataDeviceManager &wl_data_device_manager() {
    return kDisplay->wl_data_device_manager_;
  }

  static const egl::Display &egl_display() {
    return kDisplay->egl_display_;
  }

 private:

  void OnError(void *object_id,
               uint32_t code,
               const char *message);

  void OnDeleteId(uint32_t id);

  void OnGlobal(uint32_t id,
                const char *interface,
                uint32_t version);

  void OnGlobalRemove(uint32_t name);

  void OnFormat(uint32_t format);

  void OnXdgShellPing(uint32_t serial);

  Display();

  virtual ~Display();

  void Connect(const char *name = NULL);

  void Disconnect() noexcept;

  void AddOutput(Output *output, int index = 0);

  void DestroyOutput(uint32_t id);

  void AddInput(Input *input, int index = 0);

  /**
    * @brief Initialize the idle task list
    */
  void InitializeSurfaceTaskList();

  /**
   * @brief Destroy the redraw task list
   */
  void ClearSurfaceTaskList();

  void InitializeCursors();

  void ReleaseCursors();

  static const Task *surface_task_head() {
    return &kDisplay->surface_task_head_;
  }

  static const Task *surface_task_tail() {
    return &kDisplay->surface_task_tail_;
  }

  wayland::Display wl_display_;
  wayland::Registry wl_registry_;
  wayland::Compositor wl_compositor_;
  wayland::SubCompositor wl_subcompositor_;
  wayland::Shm wl_shm_;
  wayland::Shell wl_shell_;
  wayland::XdgShell xdg_shell_;  /* xdg shell v6 */
  wayland::CursorTheme wl_cursor_theme_;
  wayland::DataDeviceManager wl_data_device_manager_;

//  struct xkb_context *xkb_context_;

  int display_fd_;

  egl::Display egl_display_;

  /* output list */
  Output *first_output_;
  Output *last_output_;
  int outputs_count_;

  /* Seat list */
  Input *first_input_;
  Input *last_input_;
  int inputs_count_;

  /* window list */
  AbstractWindow *first_window_;
  AbstractWindow *last_window_;
  int windows_count_;

  /* idle task list */

  Task surface_task_head_;
  Task surface_task_tail_;

  std::list<Global *> globals_;
  std::set<uint32_t> pixel_formats_;

  Signal<const Global &> unregister_;

  std::vector<Cursor *> cursors_;

  static Display *kDisplay;

};

} // namespace skland

#endif // SKLAND_GUI_DISPLAY_HPP_
