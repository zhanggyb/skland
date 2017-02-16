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

#include <skland/gui/surface-holder.hpp>
#include <skland/core/defines.hpp>

namespace skland {

SurfaceHolder::SurfaceHolder(AbstractView *view, const Margin &margin)
    : Trackable(), surface_(nullptr) {
  surface_ = new Surface(view, margin);
  ++surface_->reference_count_;
  surface_->destroying_.Connect(this, &SurfaceHolder::OnViewSurfaceDestroying);
}

SurfaceHolder::SurfaceHolder(Surface *surface)
    : Trackable(), surface_(surface) {
  DBG_ASSERT(surface_);
  ++surface_->reference_count_;
  surface_->destroying_.Connect(this, &SurfaceHolder::OnViewSurfaceDestroying);
}

SurfaceHolder::SurfaceHolder(const SurfaceHolder &other)
    : Trackable(), surface_(other.surface_) {
  DBG_ASSERT(surface_);
  ++surface_->reference_count_;
  surface_->destroying_.Connect(this, &SurfaceHolder::OnViewSurfaceDestroying);
}

SurfaceHolder::~SurfaceHolder() {
  if (surface_ && (--surface_->reference_count_ == 0)) {
    delete surface_;
  }
}

SurfaceHolder &SurfaceHolder::operator=(const SurfaceHolder &other) {
  surface_ = other.surface_;
  ++surface_->reference_count_;
  return *this;
}

void SurfaceHolder::SetParent(Surface *parent) {
  DBG_ASSERT(surface_->parent_ == nullptr &&
      surface_->up_ == nullptr &&
      surface_->down_ == nullptr);

  surface_->parent_ = parent;

  Surface *tmp = parent;
  Surface *sibling = nullptr;
  do {
    sibling = tmp;
    tmp = tmp->above_;
    if (nullptr == tmp || tmp->parent_ != parent) break;
  } while (true);
  InsertAbove(sibling);
}

void SurfaceHolder::MoveAbove(Surface *dst) {
  Surface *top = surface_;
  Surface *bottom = surface_;
  Surface *tmp = nullptr;

  tmp = surface_;
  while (tmp->above_ && (tmp->above_->parent_ != surface_->parent_)) {
    top = tmp;
    tmp = tmp->above_;
  }

  tmp = surface_;
  while (tmp->below_ && (tmp->below_->parent_ != surface_->parent_)) {
    bottom = tmp;
    tmp = tmp->below_;
  }

  if (top == bottom) {
    if (surface_->above_) surface_->above_->below_ = surface_->below_;
    if (surface_->below_) surface_->below_->above_ = surface_->above_;

    surface_->above_ = dst->above_;
    surface_->below_ = dst;
    if (dst->above_) dst->above_->below_ = surface_;
    dst->above_ = surface_;
  } else {
    if (top->above_) top->above_->below_ = bottom->below_;
    if (bottom->below_) bottom->below_->above_ = top->above_;

    top->above_ = dst->above_;
    bottom->below_ = dst;
    if (dst->above_) dst->above_->below_ = top;
    dst->above_ = bottom;
  }
}

void SurfaceHolder::MoveBelow(Surface *dst) {
  Surface *top = surface_;
  Surface *bottom = surface_;
  Surface *tmp = nullptr;

  tmp = surface_;
  while (tmp->above_ && (tmp->above_->parent_ != surface_->parent_)) {
    top = tmp;
    tmp = tmp->above_;
  }

  tmp = surface_;
  while (tmp->below_ && (tmp->below_->parent_ != surface_->parent_)) {
    bottom = tmp;
    tmp = tmp->below_;
  }

  if (top == bottom) {
    if (surface_->above_) surface_->above_->below_ = surface_->below_;
    if (surface_->below_) surface_->below_->above_ = surface_->above_;

    surface_->above_ = dst;
    surface_->below_ = dst->below_;
    if (dst->below_) dst->below_->above_ = surface_;
    dst->below_ = surface_;
  } else {
    if (top->above_) top->above_->below_ = bottom->below_;
    if (bottom->below_) bottom->below_->above_ = top->above_;

    top->above_ = dst;
    bottom->below_ = dst->below_;
    if (dst->below_) dst->below_->above_ = bottom;
    dst->below_ = top;
  }
}

void SurfaceHolder::PushShellSurface() {
  DBG_ASSERT(nullptr == surface_->parent_);
  DBG_ASSERT(nullptr == surface_->up_);
  DBG_ASSERT(nullptr == surface_->down_);

  DBG_ASSERT(Surface::kShellSurfaceCount >= 0);

  if (Surface::kTop) {
    Surface::kTop->up_ = surface_;
    surface_->down_ = Surface::kTop;
    Surface::kTop = surface_;
  } else {
    DBG_ASSERT(Surface::kShellSurfaceCount == 0);
    DBG_ASSERT(nullptr == Surface::kBottom);
    Surface::kBottom = surface_;
    Surface::kTop = surface_;
  }

  Surface::kShellSurfaceCount++;
}

void SurfaceHolder::RemoveShellSurface() {
  DBG_ASSERT(nullptr == surface_->parent_);

  if (surface_->up_) {
    surface_->up_->down_ = surface_->down_;
  } else {
    DBG_ASSERT(Surface::kTop == surface_);
    Surface::kTop = surface_->down_;
  }

  if (surface_->down_) {
    surface_->down_->up_ = surface_->up_;
  } else {
    DBG_ASSERT(Surface::kBottom == surface_);
    Surface::kBottom = surface_->up_;
  }

  surface_->up_ = nullptr;
  surface_->down_ = nullptr;
  Surface::kShellSurfaceCount--;
  DBG_ASSERT(Surface::kShellSurfaceCount >= 0);
}

void SurfaceHolder::InsertAbove(Surface *sibling) {
  DBG_ASSERT(surface_->parent_ == sibling->parent_ || surface_ == sibling->parent_
                 || surface_->parent_ == sibling);
  if (sibling->above_) sibling->above_->below_ = surface_;
  surface_->above_ = sibling->above_;
  sibling->above_ = surface_;
  surface_->below_ = sibling;
}

void SurfaceHolder::InsertBelow(Surface *sibling) {
  DBG_ASSERT(surface_->parent_ == sibling->parent_ || surface_ == sibling->parent_
                 || surface_->parent_ == sibling);
  if (sibling->below_) sibling->below_->above_ = surface_;
  surface_->below_ = sibling->below_;
  sibling->below_ = surface_;
  surface_->above_ = sibling;
}

void SurfaceHolder::OnViewSurfaceDestroying(SLOT slot) {
  surface_destroying_.Emit();
  --surface_->reference_count_;
  DBG_ASSERT(surface_->reference_count_ >= 0);
  DBG_ASSERT(surface_->is_destroying_);
  surface_ = nullptr;
}

}
