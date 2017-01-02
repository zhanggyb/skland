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
#include <skland/gui/abstract-window.hpp>

#include "internal/view-task.hpp"
#include "internal/redraw-task.hpp"

namespace skland {

AbstractView::AbstractView()
    : AbstractView(400, 300) {

}

AbstractView::AbstractView(int width, int height)
    : Object(),
      visible_(false),
      geometry_(width, height) {
  redraw_task_.reset(new RedrawTask(this, nullptr));
  mouse_task_.reset(new ViewTask(this));
}

AbstractView::~AbstractView() {
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
  if (surface_) return surface_.get();

  AbstractSurface *surface = nullptr;
  AbstractView *p = static_cast<AbstractView *>(parent());
  while (p) {
    if (p->surface_) {
      surface = p->surface_.get();
      break;
    }
    p = static_cast<AbstractView *>(p->parent());
  }

  return surface;
}

AbstractWindow *AbstractView::GetWindow() const {
  return dynamic_cast<AbstractWindow *>(GetSurface()->view());
}

void AbstractView::SetSurface(AbstractSurface *surface) {
  if (surface_.get() == surface) {
    DBG_ASSERT(surface->view_ == this);
    return;
  }

  // TODO: inform original view

  surface_.reset(surface);

  if (surface_) {
    surface_->view_ = this;
    surface_->OnSetup();
  }
}

void AbstractView::Damage(const Rect &rect) {
  AbstractSurface *surface = GetSurface();
  if (surface) {
    surface->Damage((int) rect.x() + surface->margin().left,
                    (int) rect.y() + surface->margin().top,
                    (int) rect.width(),
                    (int) rect.height());
    surface->Commit();
  }
}

void AbstractView::Update() {
  if (redraw_task_->IsLinked()) return;

  OnUpdate(this);
}

void AbstractView::UpdateAll() {
  Update();
  for (AbstractView *sub = last_subview(); sub; sub = sub->previous_view()) {
    sub->UpdateAll();
  }
}

void AbstractView::OnUpdate(AbstractView *view) {
  if (redraw_task_->IsLinked()) {
    DBG_ASSERT(view != this);
    // This view is going to be redrawn, just push back the task of the sub view

    redraw_task_->PushBack(view->redraw_task_.get());
    view->redraw_task_->canvas = redraw_task_->canvas;
    return;
  }

  if (parent())
    parent_view()->OnUpdate(view);
}

}
