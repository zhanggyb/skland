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

#include <skland/gui/sub-surface.hpp>
#include <skland/core/defines.hpp>
#include <skland/gui/surface.hpp>

#include "internal/display-proxy.hpp"

namespace skland {

Surface *SubSurface::Create(Surface *parent, AbstractEventHandler *view, const Margin &margin) {
  Surface *surface = new Surface(view, margin);
  surface->role_.sub_surface = new SubSurface(surface, parent);
  return surface;
}

SubSurface *SubSurface::Get(const Surface *surface) {
  if (nullptr == surface->parent_) return nullptr;

  return surface->role_.sub_surface;
}

SubSurface::SubSurface(Surface *surface, Surface *parent)
    : surface_(surface) {
  DBG_ASSERT(surface_);
  DBG_ASSERT(parent);
  wl_sub_surface_.Setup(DisplayProxy().wl_subcompositor(),
                        surface_->wl_surface_,
                        parent->wl_surface_);
  SetParent(parent);
}

SubSurface::~SubSurface() {
  DBG_ASSERT(surface_->role_.sub_surface == this);

  // Delete all sub surfaces of this one:
  Surface *p = nullptr;
  Surface *tmp = nullptr;

  p = surface_->above_;
  while (p && p->parent_ == surface_) {
    tmp = p->above_;
    delete p;
    p = tmp;
  }

  p = surface_->below_;
  while (p && p->parent_ == surface_) {
    tmp = p->below_;
    delete p;
    p = tmp;
  }

  // Break the link node
  if (surface_->above_) surface_->above_->below_ = surface_->below_;
  if (surface_->below_) surface_->below_->above_ = surface_->above_;

  wl_sub_surface_.Destroy();
  surface_->role_.sub_surface = nullptr;
}

void SubSurface::PlaceAbove(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->parent() == sibling->parent() ||
      surface_ == sibling->parent() ||
      surface_->parent() == sibling) {
    wl_sub_surface_.PlaceAbove(sibling->wl_surface_);
    MoveAbove(sibling);
  }
}

void SubSurface::PlaceBelow(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->parent() == sibling->parent() ||
      surface_ == sibling->parent() ||
      surface_->parent() == sibling) {
    wl_sub_surface_.PlaceBelow(sibling->wl_surface_);
    MoveBelow(sibling);
  }
}

void SubSurface::SetRelativePosition(int x, int y) {
  wl_sub_surface_.SetPosition(x, y);
  surface_->relative_position_.x = x;
  surface_->relative_position_.y = y;
}

void SubSurface::SetWindowPosition(int x, int y) {
  Point parent_global_position = surface_->parent()->GetWindowPosition();
  int local_x = x - parent_global_position.x;
  int local_y = y - parent_global_position.y;
  wl_sub_surface_.SetPosition(local_x, local_y);
  surface_->relative_position_.x = x;
  surface_->relative_position_.y = y;
}

void SubSurface::SetParent(Surface *parent) {
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

void SubSurface::MoveAbove(Surface *dst) {
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

void SubSurface::MoveBelow(Surface *dst) {
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

void SubSurface::InsertAbove(Surface *sibling) {
  DBG_ASSERT(surface_->parent_ == sibling->parent_ || surface_ == sibling->parent_
                 || surface_->parent_ == sibling);
  if (sibling->above_) sibling->above_->below_ = surface_;
  surface_->above_ = sibling->above_;
  sibling->above_ = surface_;
  surface_->below_ = sibling;
}

void SubSurface::InsertBelow(Surface *sibling) {
  DBG_ASSERT(surface_->parent_ == sibling->parent_ || surface_ == sibling->parent_
                 || surface_->parent_ == sibling);
  if (sibling->below_) sibling->below_->above_ = surface_;
  surface_->below_ = sibling->below_;
  sibling->below_ = surface_;
  surface_->above_ = sibling;
}

}
