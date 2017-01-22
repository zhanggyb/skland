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

#include <skland/gui/abstract-surface.hpp>

#include <skland/gui/display.hpp>
#include <skland/gui/abstract-view.hpp>

namespace skland {

AbstractSurface::AbstractSurface(AbstractView *view, const Margin &margin)
    : parent_(nullptr),
      up_(nullptr),
      down_(nullptr),
      view_(view),
      margin_(margin),
      buffer_transform_(WL_OUTPUT_TRANSFORM_NORMAL),
      buffer_scale_(1),
      is_user_data_set_(false) {
  DBG_ASSERT(nullptr != view_);
  wl_surface_.enter().Set(this, &AbstractSurface::OnEnter);
  wl_surface_.leave().Set(this, &AbstractSurface::OnLeave);
  wl_surface_.Setup(Display::wl_compositor());
}

AbstractSurface::~AbstractSurface() {
  // Delete all sub surfaces of this one:
  AbstractSurface *p = nullptr;
  AbstractSurface *tmp = nullptr;

  p = up_;
  while (p && p->parent_ == this) {
    tmp = p->up_;
    delete p;
    p = tmp;
  }

  p = down_;
  while (p && p->parent_ == this) {
    tmp = p->down_;
    delete p;
    p = tmp;
  }

  // Break the link node
  if (up_) up_->down_ = down_;
  if (down_) down_->up_ = up_;
}

void AbstractSurface::AddSubSurface(AbstractSurface *subsurface, int pos) {
  if (subsurface == this || subsurface->parent_) return;

  DBG_ASSERT(nullptr == subsurface->parent_);
  DBG_ASSERT(nullptr == subsurface->up_);
  DBG_ASSERT(nullptr == subsurface->down_);
  DBG_ASSERT(!subsurface->wl_sub_surface_.IsValid());

  subsurface->wl_sub_surface_.Setup(Display::wl_subcompositor(),
                                    subsurface->wl_surface_,
                                    this->wl_surface_);
  subsurface->parent_ = this;

  AbstractSurface *tmp = this;
  AbstractSurface *p = this;
  if (pos >= 0) {
    do {
      p = tmp;
      tmp = tmp->down_;
      if (nullptr == tmp || tmp->parent_ != this) break;
      pos--;
    } while (pos >= 0);
    p->InsertFront(subsurface);
    subsurface->wl_sub_surface_.PlaceAbove(p->wl_surface_);
  } else {
    do {
      p = tmp;
      tmp = tmp->up_;
      if (nullptr == tmp || tmp->parent_ != this) break;
      pos++;
    } while (pos < 0);
    p->InsertBack(subsurface);
    subsurface->wl_sub_surface_.PlaceBelow(p->wl_surface_);
  }
}

void AbstractSurface::SetSync() const {
  if (wl_sub_surface_.IsValid()) {
    wl_sub_surface_.SetSync();
  }
}

void AbstractSurface::SetDesync() const {
  if (wl_sub_surface_.IsValid()) {
    wl_sub_surface_.SetDesync();
  }
}

void AbstractSurface::Commit() const {
  wl_surface_.Commit();
  if (parent_) {
    parent_->Commit();
  }
}

void AbstractSurface::PlaceAbove(AbstractSurface *sibling) {
  if (sibling == this) return;

  if (parent_ == sibling->parent_) {
    DBG_ASSERT(wl_sub_surface_.IsValid());
    wl_sub_surface_.PlaceAbove(sibling->wl_surface_);
    MoveAbove(sibling, this);
  } else if (this == sibling->parent_) {
    DBG_ASSERT(sibling->wl_sub_surface_.IsValid());
    sibling->wl_sub_surface_.PlaceBelow(wl_surface_);
    MoveAbove(sibling, this);
  } else if (parent_ == sibling) {
    DBG_ASSERT(wl_sub_surface_.IsValid());
    wl_sub_surface_.PlaceAbove(sibling->wl_surface_);
    MoveAbove(sibling, this);
  }
}

void AbstractSurface::PlaceBelow(AbstractSurface *sibling) {
  if (sibling == this) return;

  if (parent_ == sibling->parent_) {
    DBG_ASSERT(wl_sub_surface_.IsValid());
    wl_sub_surface_.PlaceBelow(sibling->wl_surface_);
    MoveBelow(sibling, this);
  } else if (this == sibling->parent_) {
    DBG_ASSERT(sibling->wl_sub_surface_.IsValid());
    sibling->wl_sub_surface_.PlaceAbove(wl_surface_);
    MoveBelow(sibling, this);
  } else if (parent_ == sibling) {
    DBG_ASSERT(wl_sub_surface_.IsValid());
    wl_sub_surface_.PlaceBelow(sibling->wl_surface_);
    MoveBelow(sibling, this);
  }
}

void AbstractSurface::SetPosition(int x, int y) {
  if (wl_sub_surface_.IsValid()) {
    wl_sub_surface_.SetPosition(x, y);
  }
}

void AbstractSurface::InsertFront(AbstractSurface *surface) {
  if (up_) up_->down_ = surface;
  surface->up_ = up_;
  up_ = surface;
  surface->down_ = this;
}

void AbstractSurface::InsertBack(AbstractSurface *surface) {
  if (down_) down_->up_ = surface;
  surface->down_ = down_;
  down_ = surface;
  surface->up_ = this;
}

void AbstractSurface::MoveBelow(AbstractSurface *surface_a, AbstractSurface *surface_b) {
  AbstractSurface *top = surface_b;
  AbstractSurface *bottom = surface_b;
  AbstractSurface *tmp = nullptr;

  tmp = surface_b;
  while (tmp->up_ && (tmp->up_->parent_ != surface_b->parent_)) {
    top = tmp;
    tmp = tmp->up_;
  }

  tmp = surface_b;
  while (tmp->down_ && (tmp->down_->parent_ != surface_b->parent_)) {
    bottom = tmp;
    tmp = tmp->down_;
  }

  if (top == bottom) {
    if (surface_b->up_) surface_b->up_->down_ = surface_b->down_;
    if (surface_b->down_) surface_b->down_->up_ = surface_b->up_;

    surface_b->up_ = surface_a;
    surface_b->down_ = surface_a->down_;
    if (surface_a->down_) surface_a->down_->up_ = surface_b;
    surface_a->down_ = surface_b;
  } else {
    if (top->up_) top->up_->down_ = bottom->down_;
    if (bottom->down_) bottom->down_->up_ = top->up_;

    top->up_ = surface_a;
    bottom->down_ = surface_a->down_;
    if (surface_a->down_) surface_a->down_->up_ = bottom;
    surface_a->down_ = top;
  }
}

void AbstractSurface::MoveAbove(AbstractSurface *surface_a, AbstractSurface *surface_b) {
  AbstractSurface *top = surface_b;
  AbstractSurface *bottom = surface_b;
  AbstractSurface *tmp = nullptr;

  tmp = surface_b;
  while (tmp->up_ && (tmp->up_->parent_ != surface_b->parent_)) {
    top = tmp;
    tmp = tmp->up_;
  }

  tmp = surface_b;
  while (tmp->down_ && (tmp->down_->parent_ != surface_b->parent_)) {
    bottom = tmp;
    tmp = tmp->down_;
  }

  if (top == bottom) {
    if (surface_b->up_) surface_b->up_->down_ = surface_b->down_;
    if (surface_b->down_) surface_b->down_->up_ = surface_b->up_;

    surface_b->up_ = surface_a->up_;
    surface_b->down_ = surface_a;
    if (surface_a->up_) surface_a->up_->down_ = surface_b;
    surface_a->up_ = surface_b;
  } else {
    if (top->up_) top->up_->down_ = bottom->down_;
    if (bottom->down_) bottom->down_->up_ = top->up_;

    top->up_ = surface_a->up_;
    bottom->down_ = surface_a;
    if (surface_a->up_) surface_a->up_->down_ = top;
    surface_a->up_ = bottom;
  }
}

void AbstractSurface::OnEnter(struct wl_output *wl_output) {
  if (!is_user_data_set_) {
    wl_surface_.SetUserData(this);
    is_user_data_set_ = true;
  }
  // TODO: call function in view_
}

void AbstractSurface::OnLeave(struct wl_output *wl_output) {
  // TODO: call function in view_
}

}
