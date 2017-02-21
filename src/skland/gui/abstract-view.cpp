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

#include <skland/gui/abstract-view.hpp>

#include <skland/core/numeric.hpp>
#include <skland/gui/abstract-window.hpp>
#include <skland/gui/mouse-event.hpp>

#include "internal/view-task.hpp"
#include "internal/redraw-task.hpp"

namespace skland {

Task AbstractView::kRedrawTaskHead;
Task AbstractView::kRedrawTaskTail;

AbstractView::AbstractView()
    : AbstractView(400, 300) {

}

AbstractView::AbstractView(int width, int height)
    : Object(),
      visible_(false),
      geometry_(width, height),
      is_damaged_(false) {
  redraw_task_.reset(new RedrawTask(this));
  mouse_task_.reset(new ViewTask(this));
  mouse_motion_task_.reset(new ViewTask(this));
}

AbstractView::~AbstractView() {

}

void AbstractView::MoveTo(int x, int y) {
  // TODO: check and re-layout

  set_position(x, y);
}

void AbstractView::Resize(int width, int height) {
  Size min = GetMinimalSize();
  Size max = GetMaximalSize();

  DBG_ASSERT(min.width < max.height && min.height < max.height);

  width = clamp(width, min.width, max.width);
  height = clamp(height, min.height, max.height);

  if (geometry_.width() != width || geometry_.height() != height) {
    OnResize(width, height);
  }
}

void AbstractView::Update() {
  OnUpdate(this);
}

bool AbstractView::Contain(int x, int y) const {
  return geometry_.Contain(x, y);
}

void AbstractView::UpdateAll() {
  Update();
  for (AbstractView *sub = last_subview(); sub; sub = sub->previous_view()) {
    sub->UpdateAll();
  }
}

void AbstractView::OnUpdate(AbstractView *view) {
  if (redraw_task_->IsLinked() && (view != this)) {
    // This view is going to be redrawn, just push back the task of the sub view

    redraw_task_->PushBack(view->redraw_task_.get());
    view->redraw_task_->context = redraw_task_->context;
    return;
  }

  if (parent())
    parent_view()->OnUpdate(view);
}

Surface *AbstractView::OnGetSurface(const AbstractView *view) const {
  if (view->parent_view())
    return view->parent_view()->OnGetSurface(view);

  return nullptr;
}

void AbstractView::TrackMouseMotion(MouseEvent *event) {
  if (mouse_motion_task_->IsLinked()) return;

  AbstractView *window = event->surface()->view();

  ViewTask *task = window->mouse_motion_task_.get();
  while (task->next()) {
    task = static_cast<ViewTask *>(task->next());
  }
  task->PushBack(mouse_motion_task_.get());
}

void AbstractView::UntrackMouseMotion() {
  mouse_motion_task_->Unlink();
}

Surface *AbstractView::GetSurface(const AbstractView *view) {
  return view->OnGetSurface(view);
}

void AbstractView::Damage(AbstractView *view, int surface_x, int surface_y, int width, int height) {
  view->is_damaged_ = true;
  view->damaged_region_.l = surface_x;
  view->damaged_region_.t = surface_y;
  view->damaged_region_.Resize(width, height);
}

void AbstractView::InitializeRedrawTaskList() {
  kRedrawTaskHead.PushBack(&kRedrawTaskTail);
}

void AbstractView::ClearRedrawTaskList() {
  Task *task = kRedrawTaskHead.next();
  Task *next_task = nullptr;
  while (task != &kRedrawTaskTail) {
    next_task = task->next();
    task->Unlink();
    task = next_task;
  }
}

}
