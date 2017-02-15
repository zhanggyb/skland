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

#include <skland/gui/view-surface-holder.hpp>
#include <skland/core/defines.hpp>

namespace skland {

ViewSurfaceHolder::ViewSurfaceHolder(AbstractView *view, const Margin &margin)
    : Trackable(), view_surface_(nullptr) {
  view_surface_ = new ViewSurface(view, margin);
  ++view_surface_->reference_count_;
  view_surface_->destroying_.Connect(this, &ViewSurfaceHolder::OnViewSurfaceDestroying);
}

ViewSurfaceHolder::ViewSurfaceHolder(ViewSurface *surface)
    : Trackable(), view_surface_(surface) {
  DBG_ASSERT(view_surface_);
  ++view_surface_->reference_count_;
  view_surface_->destroying_.Connect(this, &ViewSurfaceHolder::OnViewSurfaceDestroying);
}

ViewSurfaceHolder::ViewSurfaceHolder(const ViewSurfaceHolder &other)
    : Trackable(), view_surface_(other.view_surface_) {
  DBG_ASSERT(view_surface_);
  ++view_surface_->reference_count_;
  view_surface_->destroying_.Connect(this, &ViewSurfaceHolder::OnViewSurfaceDestroying);
}

ViewSurfaceHolder::~ViewSurfaceHolder() {
  if (view_surface_ && (--view_surface_->reference_count_ == 0)) {
    delete view_surface_;
  }
}

ViewSurfaceHolder &ViewSurfaceHolder::operator=(const ViewSurfaceHolder &other) {
  view_surface_ = other.view_surface_;
  ++view_surface_->reference_count_;
  return *this;
}

void ViewSurfaceHolder::SetParent(ViewSurface *parent) {
  DBG_ASSERT(view_surface_->parent_ == nullptr &&
      view_surface_->up_ == nullptr &&
      view_surface_->down_ == nullptr);

  view_surface_->parent_ = parent;

  ViewSurface *tmp = parent;
  ViewSurface *sibling = nullptr;
  do {
    sibling = tmp;
    tmp = tmp->above_;
    if (nullptr == tmp || tmp->parent_ != parent) break;
  } while (true);
  InsertAbove(sibling);
}

void ViewSurfaceHolder::MoveAbove(ViewSurface *dst) {
  ViewSurface *top = view_surface_;
  ViewSurface *bottom = view_surface_;
  ViewSurface *tmp = nullptr;

  tmp = view_surface_;
  while (tmp->above_ && (tmp->above_->parent_ != view_surface_->parent_)) {
    top = tmp;
    tmp = tmp->above_;
  }

  tmp = view_surface_;
  while (tmp->below_ && (tmp->below_->parent_ != view_surface_->parent_)) {
    bottom = tmp;
    tmp = tmp->below_;
  }

  if (top == bottom) {
    if (view_surface_->above_) view_surface_->above_->below_ = view_surface_->below_;
    if (view_surface_->below_) view_surface_->below_->above_ = view_surface_->above_;

    view_surface_->above_ = dst->above_;
    view_surface_->below_ = dst;
    if (dst->above_) dst->above_->below_ = view_surface_;
    dst->above_ = view_surface_;
  } else {
    if (top->above_) top->above_->below_ = bottom->below_;
    if (bottom->below_) bottom->below_->above_ = top->above_;

    top->above_ = dst->above_;
    bottom->below_ = dst;
    if (dst->above_) dst->above_->below_ = top;
    dst->above_ = bottom;
  }
}

void ViewSurfaceHolder::MoveBelow(ViewSurface *dst) {
  ViewSurface *top = view_surface_;
  ViewSurface *bottom = view_surface_;
  ViewSurface *tmp = nullptr;

  tmp = view_surface_;
  while (tmp->above_ && (tmp->above_->parent_ != view_surface_->parent_)) {
    top = tmp;
    tmp = tmp->above_;
  }

  tmp = view_surface_;
  while (tmp->below_ && (tmp->below_->parent_ != view_surface_->parent_)) {
    bottom = tmp;
    tmp = tmp->below_;
  }

  if (top == bottom) {
    if (view_surface_->above_) view_surface_->above_->below_ = view_surface_->below_;
    if (view_surface_->below_) view_surface_->below_->above_ = view_surface_->above_;

    view_surface_->above_ = dst;
    view_surface_->below_ = dst->below_;
    if (dst->below_) dst->below_->above_ = view_surface_;
    dst->below_ = view_surface_;
  } else {
    if (top->above_) top->above_->below_ = bottom->below_;
    if (bottom->below_) bottom->below_->above_ = top->above_;

    top->above_ = dst;
    bottom->below_ = dst->below_;
    if (dst->below_) dst->below_->above_ = bottom;
    dst->below_ = top;
  }
}

void ViewSurfaceHolder::PushShellSurface() {
  DBG_ASSERT(nullptr == view_surface_->parent_);
  DBG_ASSERT(nullptr == view_surface_->up_);
  DBG_ASSERT(nullptr == view_surface_->down_);

  DBG_ASSERT(ViewSurface::kShellSurfaceCount >= 0);

  if (ViewSurface::kTop) {
    ViewSurface::kTop->up_ = view_surface_;
    view_surface_->down_ = ViewSurface::kTop;
    ViewSurface::kTop = view_surface_;
  } else {
    DBG_ASSERT(ViewSurface::kShellSurfaceCount == 0);
    DBG_ASSERT(nullptr == ViewSurface::kBottom);
    ViewSurface::kBottom = view_surface_;
    ViewSurface::kTop = view_surface_;
  }

  ViewSurface::kShellSurfaceCount++;
}

void ViewSurfaceHolder::RemoveShellSurface() {
  DBG_ASSERT(nullptr == view_surface_->parent_);

  if (view_surface_->up_) {
    view_surface_->up_->down_ = view_surface_->down_;
  } else {
    DBG_ASSERT(ViewSurface::kTop == view_surface_);
    ViewSurface::kTop = view_surface_->down_;
  }

  if (view_surface_->down_) {
    view_surface_->down_->up_ = view_surface_->up_;
  } else {
    DBG_ASSERT(ViewSurface::kBottom == view_surface_);
    ViewSurface::kBottom = view_surface_->up_;
  }

  view_surface_->up_ = nullptr;
  view_surface_->down_ = nullptr;
  ViewSurface::kShellSurfaceCount--;
  DBG_ASSERT(ViewSurface::kShellSurfaceCount >= 0);
}

void ViewSurfaceHolder::InsertAbove(ViewSurface *sibling) {
  DBG_ASSERT(view_surface_->parent_ == sibling->parent_ || view_surface_ == sibling->parent_
                 || view_surface_->parent_ == sibling);
  if (sibling->above_) sibling->above_->below_ = view_surface_;
  view_surface_->above_ = sibling->above_;
  sibling->above_ = view_surface_;
  view_surface_->below_ = sibling;
}

void ViewSurfaceHolder::InsertBelow(ViewSurface *sibling) {
  DBG_ASSERT(view_surface_->parent_ == sibling->parent_ || view_surface_ == sibling->parent_
                 || view_surface_->parent_ == sibling);
  if (sibling->below_) sibling->below_->above_ = view_surface_;
  view_surface_->below_ = sibling->below_;
  sibling->below_ = view_surface_;
  view_surface_->above_ = sibling;
}

void ViewSurfaceHolder::OnViewSurfaceDestroying(SLOT slot) {
  view_surface_destroying_.Emit();
  --view_surface_->reference_count_;
  DBG_ASSERT(view_surface_->reference_count_ >= 0);
  DBG_ASSERT(view_surface_->is_destroying_);
  view_surface_ = nullptr;
}

}
