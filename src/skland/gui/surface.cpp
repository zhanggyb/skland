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

#include <skland/gui/surface.hpp>

#include <skland/gui/display.hpp>
#include <skland/gui/abstract-view.hpp>
#include <skland/gui/buffer.hpp>

#include <skland/gui/shell-surface.hpp>
#include <skland/gui/sub-surface.hpp>
#include <skland/gui/egl-surface.hpp>

#include "internal/commit-task.hpp"

namespace skland {

Surface *Surface::kTop = nullptr;
Surface *Surface::kBottom = nullptr;
int Surface::kShellSurfaceCount = 0;
Task Surface::kCommitTaskHead;
Task Surface::kCommitTaskTail;

Surface::Surface(AbstractView *view, const Margin &margin)
    : mode_(kSynchronized),
      parent_(nullptr),
      above_(nullptr),
      below_(nullptr),
      up_(nullptr),
      down_(nullptr),
      view_(view),
      margin_(margin),
      buffer_transform_(WL_OUTPUT_TRANSFORM_NORMAL),
      buffer_scale_(1),
      is_user_data_set_(false),
      egl_surface_role_(nullptr) {
  DBG_ASSERT(nullptr != view_);
  role_.placeholder = nullptr;

  wl_surface_.enter().Set(this, &Surface::OnEnter);
  wl_surface_.leave().Set(this, &Surface::OnLeave);
  wl_surface_.Setup(Display::wl_compositor());

  commit_task_.reset(new CommitTask(this));
}

Surface::~Surface() {
  if (egl_surface_role_) {
    delete egl_surface_role_;
  }

  if (nullptr == parent_) {
    delete role_.shell_surface; // deleting a shell surface will break the links to up_ and down_
  } else {
    delete role_.sub_surface; // deleting all sub surfaces and break the links to above_ and below_
  }
}

void Surface::Attach(Buffer *buffer, int32_t x, int32_t y) {
  if (nullptr != egl_surface_role_) return;

  if (nullptr == buffer) {
    wl_surface_.Attach(NULL, x, y);
  } else {
    buffer->SetPosition(x, y);
    wl_surface_.Attach(buffer->wl_buffer(), x, y);
  }
}

void Surface::Commit() {
  if (nullptr != egl_surface_role_) {
    // EGL surface does not use commit
    if (mode_ == kSynchronized) {
      // Synchronized mode need to commit the main surface
      Surface *main_surface = GetShellSurface();
      main_surface->Commit();
    }

    return;
  }

  if (commit_task_->IsLinked())
    return;

  if (nullptr == parent_) {
    kCommitTaskTail.PushFront(commit_task_.get());
    return;
  }

  if (mode_ == kSynchronized) {
    // Synchronized mode need to commit the main surface too
    Surface *main_surface = GetShellSurface();
    main_surface->Commit();
    main_surface->commit_task_->PushFront(commit_task_.get());
  } else {
    kCommitTaskTail.PushFront(commit_task_.get());
  }
}

Surface *Surface::GetShellSurface() {
  Surface *shell_surface = this;
  Surface *parent = parent_;

  while (parent) {
    shell_surface = parent;
    parent = parent->parent_;
  }

  return shell_surface;
}

Point Surface::GetWindowPosition() const {
  Point position = relative_position_;

  const Surface *parent = parent_;
  const Surface *shell_surface = this;

  while (parent) {
    position += parent->relative_position();
    if (nullptr == parent->parent_) shell_surface = parent;
    parent = parent->parent();
  }

  return position - Point(shell_surface->margin().l, shell_surface->margin().t);
}

void Surface::OnEnter(struct wl_output *wl_output) {
  if (!is_user_data_set_) {
    wl_surface_.SetUserData(this);
    is_user_data_set_ = true;
  }
  // TODO: call function in view_
}

void Surface::OnLeave(struct wl_output *wl_output) {
  // TODO: call function in view_
}

void Surface::Clear() {
  while (kShellSurfaceCount > 0) {
    AbstractView *view = kTop->view();
    delete view;
  }
}

void Surface::InitializeCommitTaskList() {
  kCommitTaskHead.PushBack(&kCommitTaskTail);
}

void Surface::ClearCommitTaskList() {
  Task *task = kCommitTaskHead.next();
  Task *next_task = nullptr;
  while (task != &kCommitTaskTail) {
    next_task = task->next();
    task->Unlink();
    task = next_task;
  }
}

}
