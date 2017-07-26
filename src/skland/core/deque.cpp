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

#include "skland/core/deque.hpp"
#include "skland/core/assert.hpp"

namespace skland {
namespace core {

Deque::Element::~Element() {
  if (nullptr != previous_) previous_->next_ = next_;
  if (nullptr != next_) next_->previous_ = previous_;
}

void Deque::Element::PushFront(Element *other) {
  if (other == this) return;

  other->Unlink();

  if (nullptr != previous_) previous_->next_ = other;
  other->previous_ = previous_;
  previous_ = other;
  other->next_ = this;
}

void Deque::Element::PushBack(Element *other) {
  if (other == this) return;

  other->Unlink();

  if (nullptr != next_) next_->previous_ = other;
  other->next_ = next_;
  next_ = other;
  other->previous_ = this;
}

void Deque::Element::Unlink() {
  if (nullptr != previous_) previous_->next_ = next_;
  if (nullptr != next_) next_->previous_ = previous_;

  previous_ = nullptr;
  next_ = nullptr;
}

// -----------------

Deque::Deque() {
  first_.next_ = &last_;
  last_.previous_ = &first_;
}

Deque::~Deque() {
  Clear();
}

void Deque::PushFront(Element *item) {
  item->Unlink();
  first_.PushBack(item);
}

void Deque::PushBack(Element *item) {
  item->Unlink();
  last_.PushFront(item);
}

void Deque::Insert(Element *item, int index) {
  if (index >= 0) {
    Element *p = first_.next_;
    while ((&last_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
    p->PushFront(item);
  } else {
    Element *p = last_.previous_;
    while ((&first_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
    p->PushBack(item);
  }
}

Deque::Element *Deque::Remove(Element *item) {
  if (nullptr == item) return nullptr;

  for (Iterator it = begin(); it != end(); ++it) {
    if (it == item) {
      it.Remove();
      return item;
    }
  }

  return nullptr;
}

size_t Deque::GetSize() const {
  size_t ret = 0;

  Element *element = first_.next_;
  while (element != &last_) {
    ++ret;
    element = element->next_;
  }

  return ret;
}

bool Deque::IsEmpty() const {
  _ASSERT(last_.previous_ == &first_);
  return first_.next_ == &last_;
}

void Deque::Clear() {
  while (first_.next_ != &last_) {
    first_.next_->Unlink();
  }
}

Deque::Element *Deque::operator[](int index) const {
  Element *p = nullptr;

  if (index >= 0) {
    p = first_.next_;
    while ((&last_ != p) && (index > 0)) {
      p = p->next_;
      index--;
    }
  } else {
    p = last_.previous_;
    while ((&first_ != p) && (index < -1)) {
      p = p->previous_;
      index++;
    }
  }

  return p;
}

} // namespace core
} // namespace skland
