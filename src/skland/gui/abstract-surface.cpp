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
      buffer_scale_(1) {
  wl_surface_.enter().Set(this, &AbstractSurface::OnEnter);
  wl_surface_.leave().Set(this, &AbstractSurface::OnLeave);
  wl_surface_.Setup(Display::wl_compositor());
}

AbstractSurface::~AbstractSurface() {
  AbstractSurface *subsurface = nullptr;

  subsurface = previous_;
  while (subsurface && subsurface->parent_ == this) {
    delete subsurface;
  }
  subsurface = next_;
  while (subsurface && subsurface->next_ == this) {
    delete subsurface;
  }

  if (previous_) previous_->next_ = next_;
  if (next_) next_->previous_ = previous_;

  if (view_) {
    DBG_ASSERT(view_->surface_ == this);
    view_->surface_ = nullptr;
  }
}

void AbstractSurface::AddSubSurface(AbstractSurface *subsurface, int pos) {
  DBG_ASSERT(nullptr == subsurface->parent_);
  DBG_ASSERT(nullptr == subsurface->previous_);
  DBG_ASSERT(nullptr == subsurface->next_);
  DBG_ASSERT(subsurface->wl_sub_surface_.IsNull());

  AbstractSurface *tmp = this;
  AbstractSurface *p = this;
  if (pos >= 0) {
    do {
      p = tmp;
      tmp = tmp->next_;
      if (nullptr == tmp || tmp->parent_ != this) break;
      pos--;
    } while (pos >= 0);
    p->PushBack(subsurface);
  } else {
    do {
      p = tmp;
      tmp = tmp->previous_;
      if (nullptr == tmp || tmp->parent_ != this) break;
      pos++;
    } while (pos < 0);
    p->PushFront(subsurface);
  }

  subsurface->wl_sub_surface_.Setup(Display::wl_subcompositor(),
                                    subsurface->wl_surface_,
                                    this->wl_surface_);
  subsurface->parent_ = this;
}

AbstractSurface *AbstractSurface::RemoveSubSurface(AbstractSurface *subsurface) {
  if (subsurface->parent_ != this)
    return nullptr;

  subsurface->Unlink();
  return subsurface;
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

void AbstractSurface::PlaceAbove(const AbstractSurface &surface) {
  if (wl_sub_surface_.IsValid()) {
    wl_sub_surface_.PlaceAbove(surface.wl_surface_);
  }
}

void AbstractSurface::PlaceBelow(const AbstractSurface &surface) {
  if (wl_sub_surface_.IsValid()) {
    wl_sub_surface_.PlaceBelow(surface.wl_surface_);
  }
}

void AbstractSurface::SetPosition(int x, int y) {
  if (wl_sub_surface_.IsValid()) {
    wl_sub_surface_.SetPosition(x, y);
  }
}

void AbstractSurface::PushFront(AbstractSurface *surface) {
  surface->Unlink();

  if (previous_) previous_->next_ = surface;
  surface->previous_ = previous_;
  previous_ = surface;
  surface->next_ = this;
}

void AbstractSurface::PushBack(AbstractSurface *surface) {
  surface->Unlink();

  if (next_) next_->previous_ = surface;
  surface->next_ = next_;
  next_ = surface;
  surface->previous_ = this;
}

void AbstractSurface::Unlink() {
  if (previous_) previous_->next_ = next_;
  if (next_) next_->previous_ = previous_;

  previous_ = nullptr;
  next_ = nullptr;
}

void AbstractSurface::OnEnter(struct wl_output *wl_output) {
  wl_surface_.SetUserData(this);
  // TODO: call function in view_
}

void AbstractSurface::OnLeave(struct wl_output *wl_output) {
  // TODO: call function in view_
}

}
