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

AbstractSurface *AbstractSurface::kTop = nullptr;
AbstractSurface *AbstractSurface::kBottom = nullptr;
int AbstractSurface::kCount = 0;

AbstractSurface::AbstractSurface(AbstractView *view, const Margin &margin)
    : parent_(nullptr),
      above_(nullptr),
      below_(nullptr),
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

  Push(this);
}

AbstractSurface::~AbstractSurface() {
  // Delete all sub surfaces of this one:
  AbstractSurface *p = nullptr;
  AbstractSurface *tmp = nullptr;

  p = above_;
  while (p && p->parent_ == this) {
    tmp = p->above_;
    delete p;
    p = tmp;
  }

  p = below_;
  while (p && p->parent_ == this) {
    tmp = p->below_;
    delete p;
    p = tmp;
  }

  // Break the link node
  if (above_) above_->below_ = below_;
  if (below_) below_->above_ = above_;

  if (nullptr == parent_)
    Remove(this);
}

void AbstractSurface::AddSubSurface(AbstractSurface *subsurface, int pos) {
  if (subsurface == this || subsurface->parent_) return;

  Remove(subsurface);

  DBG_ASSERT(nullptr == subsurface->parent_);
  DBG_ASSERT(nullptr == subsurface->above_);
  DBG_ASSERT(nullptr == subsurface->below_);
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
      tmp = tmp->below_;
      if (nullptr == tmp || tmp->parent_ != this) break;
      pos--;
    } while (pos >= 0);
    p->InsertFront(subsurface);
    subsurface->wl_sub_surface_.PlaceAbove(p->wl_surface_);
  } else {
    do {
      p = tmp;
      tmp = tmp->above_;
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
  if (above_) above_->below_ = surface;
  surface->above_ = above_;
  above_ = surface;
  surface->below_ = this;
}

void AbstractSurface::InsertBack(AbstractSurface *surface) {
  if (below_) below_->above_ = surface;
  surface->below_ = below_;
  below_ = surface;
  surface->above_ = this;
}

void AbstractSurface::MoveBelow(AbstractSurface *surface_a, AbstractSurface *surface_b) {
  AbstractSurface *top = surface_b;
  AbstractSurface *bottom = surface_b;
  AbstractSurface *tmp = nullptr;

  tmp = surface_b;
  while (tmp->above_ && (tmp->above_->parent_ != surface_b->parent_)) {
    top = tmp;
    tmp = tmp->above_;
  }

  tmp = surface_b;
  while (tmp->below_ && (tmp->below_->parent_ != surface_b->parent_)) {
    bottom = tmp;
    tmp = tmp->below_;
  }

  if (top == bottom) {
    if (surface_b->above_) surface_b->above_->below_ = surface_b->below_;
    if (surface_b->below_) surface_b->below_->above_ = surface_b->above_;

    surface_b->above_ = surface_a;
    surface_b->below_ = surface_a->below_;
    if (surface_a->below_) surface_a->below_->above_ = surface_b;
    surface_a->below_ = surface_b;
  } else {
    if (top->above_) top->above_->below_ = bottom->below_;
    if (bottom->below_) bottom->below_->above_ = top->above_;

    top->above_ = surface_a;
    bottom->below_ = surface_a->below_;
    if (surface_a->below_) surface_a->below_->above_ = bottom;
    surface_a->below_ = top;
  }
}

void AbstractSurface::MoveAbove(AbstractSurface *surface_a, AbstractSurface *surface_b) {
  AbstractSurface *top = surface_b;
  AbstractSurface *bottom = surface_b;
  AbstractSurface *tmp = nullptr;

  tmp = surface_b;
  while (tmp->above_ && (tmp->above_->parent_ != surface_b->parent_)) {
    top = tmp;
    tmp = tmp->above_;
  }

  tmp = surface_b;
  while (tmp->below_ && (tmp->below_->parent_ != surface_b->parent_)) {
    bottom = tmp;
    tmp = tmp->below_;
  }

  if (top == bottom) {
    if (surface_b->above_) surface_b->above_->below_ = surface_b->below_;
    if (surface_b->below_) surface_b->below_->above_ = surface_b->above_;

    surface_b->above_ = surface_a->above_;
    surface_b->below_ = surface_a;
    if (surface_a->above_) surface_a->above_->below_ = surface_b;
    surface_a->above_ = surface_b;
  } else {
    if (top->above_) top->above_->below_ = bottom->below_;
    if (bottom->below_) bottom->below_->above_ = top->above_;

    top->above_ = surface_a->above_;
    bottom->below_ = surface_a;
    if (surface_a->above_) surface_a->above_->below_ = top;
    surface_a->above_ = bottom;
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

void AbstractSurface::Push(AbstractSurface *surface) {
  DBG_ASSERT(nullptr == surface->parent_);
  DBG_ASSERT(nullptr == surface->up_);
  DBG_ASSERT(nullptr == surface->down_);

  DBG_ASSERT(kCount >= 0);

  if (kTop) {
    kTop->up_ = surface;
    surface->down_ = kTop;
    kTop = surface;
  } else {
    DBG_ASSERT(kCount == 0);
    DBG_ASSERT(nullptr == kBottom);
    kBottom = surface;
    kTop = surface;
  }

  kCount++;
}

void AbstractSurface::Remove(AbstractSurface *surface) {
  DBG_ASSERT(nullptr == surface->parent_);

  if (surface->up_) {
    surface->up_->down_ = surface->down_;
  } else {
    DBG_ASSERT(kTop == surface);
    kTop = surface->down_;
  }

  if (surface->down_) {
    surface->down_->up_ = surface->up_;
  } else {
    DBG_ASSERT(kBottom == surface);
    kBottom = surface->up_;
  }

  surface->up_ = nullptr;
  surface->down_ = nullptr;
  kCount--;
  DBG_ASSERT(kCount >= 0);
}

void AbstractSurface::Clear() {
  while (kCount > 0) {
    AbstractView *view = kTop->view();
    delete view;
  }
}

}
