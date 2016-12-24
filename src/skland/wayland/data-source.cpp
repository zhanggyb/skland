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

#include <skland/wayland/data-source.hpp>

namespace skland {
namespace wayland {

const struct wl_data_source_listener DataSource::kListener = {
    OnTarget,
    OnSend,
    OnCancelled,
    OnDndDropPerformed,
    OnDndFinished,
    OnAction
};

void DataSource::OnTarget(void *data, struct wl_data_source *wl_data_source, const char *mime_type) {
  DataSource *_this = static_cast<DataSource *>(data);
  if (_this->target_) _this->target_(mime_type);
}

void DataSource::OnSend(void *data, struct wl_data_source *wl_data_source, const char *mime_type, int32_t fd) {
  DataSource *_this = static_cast<DataSource *>(data);
  if (_this->send_) _this->send_(mime_type, fd);
}

void DataSource::OnCancelled(void *data, struct wl_data_source *wl_data_source) {
  DataSource *_this = static_cast<DataSource *>(data);
  if (_this->cancelled_) _this->cancelled_();
}

void DataSource::OnDndDropPerformed(void *data, struct wl_data_source *wl_data_source) {
  DataSource *_this = static_cast<DataSource *>(data);
  if (_this->dnd_drop_performed_) _this->dnd_drop_performed_();
}

void DataSource::OnDndFinished(void *data, struct wl_data_source *wl_data_source) {
  DataSource *_this = static_cast<DataSource *>(data);
  if (_this->dnd_finished_) _this->dnd_finished_();
}

void DataSource::OnAction(void *data, struct wl_data_source *wl_data_source, uint32_t dnd_action) {
  DataSource *_this = static_cast<DataSource *>(data);
  if (_this->action_) _this->action_(dnd_action);
}

}
}
