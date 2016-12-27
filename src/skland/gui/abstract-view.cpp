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
#include <skland/gui/abstract-surface.hpp>
#include <skland/gui/display.hpp>

#include "internal/view-task.hpp"
#include "internal/redraw-task.hpp"

namespace skland {

AbstractView::AbstractView()
    : AbstractView(400, 300) {

}

AbstractView::AbstractView(int width, int height)
    : Object(),
      visible_(false),
      geometry_(width, height),
      surface_(nullptr) {
  redraw_task_.reset(new RedrawTask(this, nullptr));
  mouse_task_.reset(new ViewTask(this));
}

AbstractView::~AbstractView() {
  delete surface_;
}

void AbstractView::Show() {
  if (redraw_task_->IsLinked()) {
    // TODO: check if the redraw task of this view is in correct order
    return;
  }

  OnShow();
}

void AbstractView::SetPosition(int x, int y) {
  // TODO: check and re-layout

  set_position(x, y);
}

void AbstractView::Resize(int width, int height) {
  Size min = GetMinimalSize();
  Size max = GetMaximalSize();

  DBG_ASSERT(min.width < max.height && min.height < max.height);

  width = clamp(width, min.width, max.width);
  height = clamp(height, min.height, max.height);

  if (this->width() != width || this->height() != height) {
    OnResize(width, height);
  }
}

bool AbstractView::Contain(int x, int y) const {
  return geometry_.Contain(x, y);
}

AbstractSurface *AbstractView::GetSurface() const {
  if (surface_) return surface_;

  AbstractSurface *surface = nullptr;
  AbstractView *p = static_cast<AbstractView *>(parent());
  while (p) {
    if (p->surface_) {
      surface = p->surface_;
      break;
    }
    p = static_cast<AbstractView *>(p->parent());
  }

  return surface;
}

void AbstractView::SetSurface(AbstractSurface *surface) {
  if (surface_ == surface) {
    DBG_ASSERT(surface->view() == this);
    return;
  }

  // TODO: inform original view

  surface->view_ = this;
  surface_ = surface;
  surface_->OnSetup();
}

void AbstractView::Damage(const Rect &rect) {
  AbstractSurface *surface = GetSurface();
  if (surface) {
    surface->Damage((int) rect.x() + surface->margin().left,
                    (int) rect.y() + surface->margin().top,
                    (int) rect.width(),
                    (int) rect.height());
  }
}

void AbstractView::RedrawAll() {
  AbstractSurface *surface = GetSurface();
  if (surface) {
    RedrawOnSurface(this, surface);
    RedrawSubViewsOnSurface(this, surface_);
    surface->Commit();
  }
}

void AbstractView::RedrawOnSurface(AbstractView *view, AbstractSurface *surface) {
  if (surface->canvas()) {
    static_cast<RedrawTask *>(view->redraw_task_.get())->canvas = surface->canvas().get();
    AddRedrawTask(view->redraw_task_.get());
  }
}

void AbstractView::RedrawSubViewsOnSurface(const AbstractView *parent, AbstractSurface *surface) {
  for (AbstractView *subview = parent->last_subview(); subview;
       subview = subview->previous_view()) {
    if (subview->surface_ != nullptr) {
      surface = subview->surface_;
      surface->Commit();
    }
    RedrawOnSurface(subview, surface);
    RedrawSubViewsOnSurface(subview, surface);
  }
}

void AbstractView::AddRedrawTask(ViewTask *task) {
  DBG_ASSERT(task->view != nullptr);

  // The task node after which insert the new one
  Task *insert_task = Display::idle_task_tail()->previous();

  AbstractView *view = task->view->next_view();
  if (view && view->redraw_task_->IsLinked()) { // the next sibling is waiting for redraw, insert after
    insert_task = view->redraw_task_.get();
    goto insert;
  }

  view = task->view->previous_view();
  if (view && view->redraw_task_->IsLinked()) { // the previous sibling is waiting for redraw, insert after
    insert_task = view->redraw_task_->previous();
    goto insert;
  }

  view = static_cast<AbstractView *>(task->view->parent());
  if (view && view->redraw_task_->IsLinked()) { // the parent is waiting for redraw, insert after
    insert_task = view->redraw_task_.get();
    goto insert;
  }

  insert:
  task->Unlink();
  insert_task->PushBack(task);
}

}
