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

#include "internal/abstract-event-handler-private.hpp"

namespace skland {

Task AbstractEventHandler::kRedrawTaskHead;
Task AbstractEventHandler::kRedrawTaskTail;

AbstractEventHandler::AbstractEventHandler()
    : Trackable() {
  p_.reset(new Private(this));
}

AbstractEventHandler::~AbstractEventHandler() {

}

void AbstractEventHandler::InitializeRedrawTaskList() {
  kRedrawTaskHead.PushBack(&kRedrawTaskTail);
}

void AbstractEventHandler::ClearRedrawTaskList() {
  Task *task = kRedrawTaskHead.next();
  Task *next_task = nullptr;
  while (task != &kRedrawTaskTail) {
    next_task = task->next();
    task->Unlink();
    task = next_task;
  }
}

void AbstractEventHandler::Damage(AbstractEventHandler *object, int surface_x, int surface_y, int width, int height) {
  object->p_->is_damaged_ = true;
  object->p_->damaged_region_.l = surface_x;
  object->p_->damaged_region_.t = surface_y;
  object->p_->damaged_region_.Resize(width, height);
}

}