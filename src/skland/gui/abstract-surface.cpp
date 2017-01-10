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
#include <skland/gui/buffer.hpp>
#include <skland/gui/abstract-view.hpp>

namespace skland {

AbstractSurface::AbstractSurface(const Margin &margin)
    : parent_(nullptr),
      previous_(nullptr),
      next_(nullptr),
      view_(nullptr),
      margin_(margin),
      buffer_transform_(WL_OUTPUT_TRANSFORM_NORMAL),
      buffer_scale_(1),
      is_user_data_set_(false) {
  wl_surface_.enter().Set(this, &AbstractSurface::OnEnter);
  wl_surface_.leave().Set(this, &AbstractSurface::OnLeave);
  wl_surface_.Setup(Display::wl_compositor());
}

AbstractSurface::~AbstractSurface() {
  // Delete all sub surfaces of this one:
  AbstractSurface *p = nullptr;
  AbstractSurface *tmp = nullptr;

  p = previous_;
  while (p && p->parent_ == this) {
    tmp = p->previous_;
    delete p;
    p = tmp;
  }

  p = next_;
  while (p && p->parent_ == this) {
    tmp = p->next_;
    delete p;
    p = tmp;
  }

  // Break the link node
  if (previous_) previous_->next_ = next_;
  if (next_) next_->previous_ = previous_;

  if (view_ && view_->surface_ == this) {
    view_->surface_ = nullptr;
  }
}

void AbstractSurface::AddSubSurface(AbstractSurface *subsurface, int pos) {
  if (subsurface == this || subsurface->parent_) return;

  DBG_ASSERT(nullptr == subsurface->parent_);
  DBG_ASSERT(nullptr == subsurface->previous_);
  DBG_ASSERT(nullptr == subsurface->next_);
  DBG_ASSERT(subsurface->wl_sub_surface_.IsNull());

  subsurface->wl_sub_surface_.Setup(Display::wl_subcompositor(),
                                    subsurface->wl_surface_,
                                    this->wl_surface_);
  subsurface->parent_ = this;

  AbstractSurface *tmp = this;
  AbstractSurface *p = this;
  if (pos >= 0) {
    do {
      p = tmp;
      tmp = tmp->next_;
      if (nullptr == tmp || tmp->parent_ != this) break;
      pos--;
    } while (pos >= 0);
    p->InsertFront(subsurface);
    subsurface->wl_sub_surface_.PlaceAbove(p->wl_surface_);
  } else {
    do {
      p = tmp;
      tmp = tmp->previous_;
      if (nullptr == tmp || tmp->parent_ != this) break;
      pos++;
    } while (pos < 0);
    p->InsertBack(subsurface);
    subsurface->wl_sub_surface_.PlaceBelow(p->wl_surface_);
  }
}

void AbstractSurface::Attach(Buffer *buffer, int32_t x, int32_t y) {
  if (nullptr == buffer || buffer->wl_buffer().IsNull()) {
    wl_surface_.Attach(NULL, x, y);
  } else {
    buffer->position_.x = x;
    buffer->position_.y = y;
    wl_surface_.Attach(buffer->wl_buffer(), x, y);
  }

  OnAttach(buffer);
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
  if (previous_) previous_->next_ = surface;
  surface->previous_ = previous_;
  previous_ = surface;
  surface->next_ = this;
}

void AbstractSurface::InsertBack(AbstractSurface *surface) {
  if (next_) next_->previous_ = surface;
  surface->next_ = next_;
  next_ = surface;
  surface->previous_ = this;
}

void AbstractSurface::MoveBelow(AbstractSurface *surface_a, AbstractSurface *surface_b) {
  AbstractSurface *head = surface_b;
  AbstractSurface *tail = surface_b;
  AbstractSurface *tmp = nullptr;

  tmp = surface_b;
  while (tmp->previous_ && (tmp->previous_->parent_ != surface_b->parent_)) {
    head = tmp;
    tmp = tmp->previous_;
  }

  tmp = surface_b;
  while (tmp->next_ && (tmp->next_->parent_ != surface_b->parent_)) {
    tail = tmp;
    tmp = tmp->next_;
  }

  if (head == tail) {
    if (surface_b->previous_) surface_b->previous_->next_ = surface_b->next_;
    if (surface_b->next_) surface_b->next_->previous_ = surface_b->previous_;

    surface_b->previous_ = surface_a;
    surface_b->next_ = surface_a->next_;
    if (surface_a->next_) surface_a->next_->previous_ = surface_b;
    surface_a->next_ = surface_b;
  } else {
    if (head->previous_) head->previous_->next_ = tail->next_;
    if (tail->next_) tail->next_->previous_ = head->previous_;

    head->previous_ = surface_a;
    tail->next_ = surface_a->next_;
    if (surface_a->next_) surface_a->next_->previous_ = tail;
    surface_a->next_ = head;
  }
}

void AbstractSurface::MoveAbove(AbstractSurface *surface_a, AbstractSurface *surface_b) {
  AbstractSurface *head = surface_b;
  AbstractSurface *tail = surface_b;
  AbstractSurface *tmp = nullptr;

  tmp = surface_b;
  while (tmp->previous_ && (tmp->previous_->parent_ != surface_b->parent_)) {
    head = tmp;
    tmp = tmp->previous_;
  }

  tmp = surface_b;
  while (tmp->next_ && (tmp->next_->parent_ != surface_b->parent_)) {
    tail = tmp;
    tmp = tmp->next_;
  }

  if (head == tail) {
    if (surface_b->previous_) surface_b->previous_->next_ = surface_b->next_;
    if (surface_b->next_) surface_b->next_->previous_ = surface_b->previous_;

    surface_b->previous_ = surface_a->previous_;
    surface_b->next_ = surface_a;
    if (surface_a->previous_) surface_a->previous_->next_ = surface_b;
    surface_a->previous_ = surface_b;
  } else {
    if (head->previous_) head->previous_->next_ = tail->next_;
    if (tail->next_) tail->next_->previous_ = head->previous_;

    head->previous_ = surface_a->previous_;
    tail->next_ = surface_a;
    if (surface_a->previous_) surface_a->previous_->next_ = head;
    surface_a->previous_ = tail;
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
