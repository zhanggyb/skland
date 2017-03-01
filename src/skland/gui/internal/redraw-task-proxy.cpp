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

#include "redraw-task-proxy.hpp"

#include "redraw-task.hpp"
#include "abstract-event-handler-private.hpp"

namespace skland {

void RedrawTaskProxy::SetContext(const Context &context) {
  event_handler_->p_->redraw_task.context = context;
}

void RedrawTaskProxy::CopyContextTo(AbstractEventHandler *event_handler) {
  event_handler->p_->redraw_task.context = event_handler_->p_->redraw_task.context;
}

const Context &RedrawTaskProxy::GetContext() const {
  return event_handler_->p_->redraw_task.context;
}

bool RedrawTaskProxy::IsLinked() const {
  return event_handler_->p_->redraw_task.IsLinked();
}

void RedrawTaskProxy::PushBack(AbstractEventHandler *event_handler) {
  event_handler_->p_->redraw_task.PushBack(&event_handler->p_->redraw_task);
}

void RedrawTaskProxy::PushFront(AbstractEventHandler *event_handler) {
  event_handler_->p_->redraw_task.PushFront(&event_handler->p_->redraw_task);
}

void RedrawTaskProxy::MoveToHead() {
  AbstractEventHandler::kRedrawTaskHead.PushBack(&event_handler_->p_->redraw_task);
}

void RedrawTaskProxy::MoveToTail() {
  AbstractEventHandler::kRedrawTaskTail.PushFront(&event_handler_->p_->redraw_task);
}

}