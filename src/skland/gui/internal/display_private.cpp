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

#include "display_private.hpp"

#include <skland/core/debug.hpp>
#include <skland/core/assert.hpp>

#include <skland/gui/input.hpp>
#include <skland/gui/output.hpp>

namespace skland {

const struct wl_display_listener Display::Private::kDisplayListener = {
    OnError,
    OnDeleteId
};

const struct wl_registry_listener Display::Private::kRegistryListener = {
    OnGlobal,
    OnGlobalRemove
};

const struct wl_shm_listener Display::Private::kShmListener = {
    OnFormat
};

const struct zxdg_shell_v6_listener Display::Private::kXdgShellListener = {
    OnPing
};

void Display::Private::OnFormat(void *data, struct wl_shm *shm, uint32_t format) {
  Display *_this = static_cast<Display *>(data);
  const char *text = nullptr;

  _this->pixel_formats_.insert(format);

  switch (format) {
    case WL_SHM_FORMAT_ARGB8888: {
      text = "ARGB8888";
      break;
    }
    case WL_SHM_FORMAT_XRGB8888: {
      text = "XRGB8888";
      break;
    }
    default: {
      text = "other format";
      break;
    }
  }

  fprintf(stderr, "Possible shmem format %s\n", text);
}

void Display::Private::OnError(void *data,
                               struct wl_display *wl_display,
                               void *object_id,
                               uint32_t code,
                               const char *message) {
  Display *_this = static_cast<Display *>(data);
  const char *object_name = "Unknown object";

  if (_this->p_->wl_display == object_id) {
    object_name = "wl_display";
  } else if (_this->p_->wl_compositor == object_id) {
    object_name = "wl_compositor";
  } else if (_this->p_->wl_registry == object_id) {
    object_name = "wl_registry";
  } else if (_this->p_->wl_subcompositor == object_id) {
    object_name = "wl_subcompositor";
  } else if (_this->p_->wl_shm == object_id) {
    object_name = "wl_shm";
  } else if (_this->p_->wl_shell == object_id) {
    object_name = "wl_shell";
  } else if (_this->p_->xdg_shell == object_id) {
    object_name = "xdg_shell";
  }

  _DEBUG("Error from %s: %s (%d)\n", object_name, message, code);
}

void Display::Private::OnDeleteId(void *data, struct wl_display *wl_display, uint32_t id) {
//  Display *_this = static_cast<Display *>(data);
  _DEBUG("Delete ID: %ud\n", id);
}

void Display::Private::OnGlobal(void *data,
                                struct wl_registry *registry,
                                uint32_t id,
                                const char *interface,
                                uint32_t version) {
  Display *_this = static_cast<Display *>(data);

  struct Global *global = new Global;
  global->id = id;
  global->interface = interface;
  global->version = version;
  _this->globals_.push_back(global);

  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    _this->p_->wl_compositor = static_cast<struct wl_compositor *>(wl_registry_bind(_this->p_->wl_registry,
                                                                                    id,
                                                                                    &wl_compositor_interface,
                                                                                    version));
  } else if (strcmp(interface, wl_subcompositor_interface.name) == 0) {
    _this->p_->wl_subcompositor =
        static_cast<struct wl_subcompositor *>(wl_registry_bind(_this->p_->wl_registry,
                                                                id,
                                                                &wl_subcompositor_interface,
                                                                version));
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    _this->p_->wl_shm =
        static_cast<struct wl_shm *>(wl_registry_bind(_this->p_->wl_registry,
                                                      id,
                                                      &wl_shm_interface,
                                                      version));
    wl_shm_add_listener(_this->p_->wl_shm, &Private::kShmListener, _this);

    _ASSERT(nullptr == _this->p_->wl_cursor_theme);
    _this->p_->wl_cursor_theme = wl_cursor_theme_load(NULL, 24, _this->p_->wl_shm);

    _this->InitializeCursors();
  } else if (strcmp(interface, wl_output_interface.name) == 0) {
    Output *output = new Output(id, version);
    _this->AddOutput(output);
  } else if (strcmp(interface, zxdg_shell_v6_interface.name) == 0) {
    _this->p_->xdg_shell = static_cast<struct zxdg_shell_v6 *>(wl_registry_bind(_this->p_->wl_registry,
                                                                                id,
                                                                                &zxdg_shell_v6_interface,
                                                                                version));
    zxdg_shell_v6_add_listener(_this->p_->xdg_shell, &Private::kXdgShellListener, _this);
  } else if (strcmp(interface, wl_shell_interface.name) == 0) {
    _this->p_->wl_shell = static_cast<struct wl_shell *>(wl_registry_bind(_this->p_->wl_registry,
                                                                          id, &wl_shell_interface, version));
  } else if (strcmp(interface, wl_seat_interface.name) == 0) {
    Input *input = new Input(id, version);
    _this->AddInput(input);
  } else if (strcmp(interface, wl_data_device_manager_interface.name) == 0) {
    _this->p_->wl_data_device_manager =
        static_cast<struct wl_data_device_manager *>(wl_registry_bind(_this->p_->wl_registry,
                                                                      id,
                                                                      &wl_data_device_manager_interface,
                                                                      version));
  }
}

void Display::Private::OnGlobalRemove(void *data,
                                      struct wl_registry *registry,
                                      uint32_t name) {
  Display *_this = static_cast<Display *>(data);

  for (std::list<Global *>::iterator it = _this->globals_.begin(); it != _this->globals_.end();) {
    if ((*it)->id != name) {
      it++;
      continue;
    }

    if ((*it)->interface == "wl_output") {
      _this->DestroyOutput(name);
    }

    _this->unregister_.Emit(**it);

    delete (*it);
    it = _this->globals_.erase(it);
  }
}

void Display::Private::OnPing(void *data, struct zxdg_shell_v6 *zxdg_shell_v6, uint32_t serial) {
  Display *_this = static_cast<Display *>(data);
  zxdg_shell_v6_pong(_this->p_->xdg_shell, serial);
}

}
