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

#include "../core/object.hpp"
#include "../core/types.hpp"

#include "cursor.hpp"

#include <xkbcommon/xkbcommon.h>

#include <list>
#include <vector>
#include <set>
#include <memory>

namespace skland {

// Forward declarations

class Output;
class Input;

struct Global {
  uint32_t id;
  std::string interface;
  uint32_t version;
};

/**
 * @ingroup gui
 * @brief The global display
 */
class Display : public Object {

  friend class Application;
  friend class Output;
  friend class Input;

 public:

  class Registry;

  static const Output *GetOutputAt(int index = 0);

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

 private:

  struct Private;

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

  void InitializeCursors();

  void ReleaseCursors();

  std::unique_ptr<Private> data_;

  int display_fd_;

  /* output list */
  Output *first_output_;
  Output *last_output_;
  int outputs_count_;

  /* Seat list */
  Input *first_input_;
  Input *last_input_;
  int inputs_count_;

  std::list<Global *> globals_;
  std::set<uint32_t> pixel_formats_;

  Signal<const Global &> unregister_;

  std::vector<Cursor *> cursors_;

  static Display *kDisplay;

};

} // namespace skland

#endif // SKLAND_GUI_DISPLAY_HPP_
