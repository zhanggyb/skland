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

#include "abstract-view-anchor-group.hpp"
#include "abstract-view-anchor.hpp"

namespace skland {

AbstractView::AnchorGroup::AnchorGroup(AbstractView *view, Alignment align)
    : view(view),
      first_(nullptr),
      last_(nullptr),
      count_(0),
      alignment_(align) {

}

AbstractView::AnchorGroup::~AnchorGroup() {
  Clear();
}

AbstractView::Anchor *AbstractView::AnchorGroup::operator[](int index) const {
  if (index < 0) index = count_ + index;

  Anchor *anchor = nullptr;
  int mid = (count_ + 1) / 2;

  if (index >= 0 && index < mid) {
    anchor = first_;
    while (index > 0) {
      anchor = anchor->next;
      index--;
    }
  } else if (index >= mid && index < count_) {
    anchor = last_;
    int max = count_ - 1;
    while (index < max) {
      anchor = anchor->previous;
      index++;
    }
  }

  return anchor;
}

void AbstractView::AnchorGroup::PushFront(Anchor *anchor) {
  DBG_ASSERT(nullptr == anchor->group);
  DBG_ASSERT(nullptr == anchor->previous);
  DBG_ASSERT(nullptr == anchor->next);
  DBG_ASSERT(anchor->contrary->contrary == anchor);

  if (first_) {
    first_->previous = anchor;
    anchor->next = first_;
  } else {
    DBG_ASSERT(nullptr == last_);
    DBG_ASSERT(0 == count_);
  }
  first_ = anchor;

  anchor->group = this;
  count_++;
}

void AbstractView::AnchorGroup::Insert(int index, Anchor *anchor) {
  DBG_ASSERT(nullptr == anchor->group);
  DBG_ASSERT(nullptr == anchor->previous);
  DBG_ASSERT(nullptr == anchor->next);
  DBG_ASSERT(anchor->contrary->contrary == anchor);

  if (nullptr == first_) {
    DBG_ASSERT(nullptr == last_);
    DBG_ASSERT(0 == count_);
    // child->data_->next_ = nullptr;
    // child->data_->previous_ = nullptr;
    last_ = anchor;
    first_ = anchor;
  } else {
    if (index >= 0) {
      Anchor *p = first_;
      while (p && (index > 0)) {
        p = p->next;
        index--;
      }
      if (p) {  // insert before p
        anchor->previous = p->previous;
        anchor->next = p;
        if (p->previous) p->previous->next = anchor;
        else first_ = anchor;
        p->previous = anchor;
      } else {  // push back
        last_->next = anchor;
        anchor->previous = last_;
        last_ = anchor;
        // child->data_->next_ = nullptr;
      }
    } else {
      Anchor *p = last_;
      while (p && (index < -1)) {
        p = p->previous;
        index++;
      }
      if (p) {  // insert after p
        anchor->next = p->next;
        anchor->previous = p;
        if (p->next) p->next->previous = anchor;
        else last_ = anchor;
        p->next = anchor;
      } else {  // push front
        first_->previous = anchor;
        anchor->next = first_;
        first_ = anchor;
        // child->data_->previous_ = nullptr;
      }
    }
  }

  anchor->group = this;
  count_++;

}

void AbstractView::AnchorGroup::PushBack(Anchor *anchor) {
  DBG_ASSERT(nullptr == anchor->group);
  DBG_ASSERT(nullptr == anchor->previous);
  DBG_ASSERT(nullptr == anchor->next);
  DBG_ASSERT(anchor->contrary->contrary == anchor);

  if (last_) {
    last_->next = anchor;
    anchor->previous = last_;
  } else {
    DBG_ASSERT(nullptr == first_);
    DBG_ASSERT(0 == count_);
    first_ = anchor;
  }
  last_ = anchor;

  anchor->group = this;
  count_++;
}

AbstractView::Anchor *AbstractView::AnchorGroup::Remove(Anchor *anchor) {
  if (anchor->group != this) return nullptr;

  DBG_ASSERT(count_ > 0);

  if (anchor->previous) {
    anchor->previous->next = anchor->next;
  } else {
    DBG_ASSERT(first_ == anchor);
    first_ = anchor->next;
  }

  if (anchor->next) {
    anchor->next->previous = anchor->previous;
  } else {
    DBG_ASSERT(last_ == anchor);
    last_ = anchor->previous;
  }

  count_--;
  DBG_ASSERT(count_ >= 0);

  anchor->previous = nullptr;
  anchor->next = nullptr;
  anchor->group = nullptr;

  return anchor;
}

void AbstractView::AnchorGroup::Clear() {
  Anchor *ptr = first_;
  Anchor *next = nullptr;

  while (ptr) {
    next = ptr->next;
    ptr->previous = nullptr;
    ptr->next = nullptr;
    ptr->group = nullptr;
    delete ptr;
    ptr = next;
  }

  count_ = 0;
  first_ = nullptr;
  last_ = nullptr;
}

}
