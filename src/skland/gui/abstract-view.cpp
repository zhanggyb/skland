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
#include <skland/gui/abstract-window.hpp>
#include <skland/gui/mouse-event.hpp>

#include "internal/abstract-view-private.hpp"

namespace skland {

Task AbstractView::kRedrawTaskHead;
Task AbstractView::kRedrawTaskTail;

AbstractView::AbstractView()
    : AbstractView(400, 300) {

}

AbstractView::AbstractView(int width, int height)
    : Trackable(),
      visible_(false),
      geometry_(width, height),
      previous_(nullptr),
      next_(nullptr),
      first_child_(nullptr),
      last_child_(nullptr),
      parent_(nullptr),
      children_count_(0) {
  data_.reset(new Private(this));
}

AbstractView::~AbstractView() {
  if (parent_) parent_->RemoveChild(this);
  DBG_ASSERT(previous_ == nullptr);
  DBG_ASSERT(next_ == nullptr);

  if (children_count_ > 0) {
    DBG_ASSERT(first_child_ != nullptr);
    DBG_ASSERT(last_child_ != nullptr);
    ClearChildren();
  }
  DBG_ASSERT(children_count_ == 0);
  DBG_ASSERT(first_child_ == nullptr);
  DBG_ASSERT(last_child_ == nullptr);
}

void AbstractView::MoveTo(int x, int y) {
  // TODO: check and re-layout

  set_position(x, y);
}

void AbstractView::Resize(int width, int height) {
  Size min = GetMinimalSize();
  Size max = GetMaximalSize();

  DBG_ASSERT(min.width < max.height && min.height < max.height);

  width = clamp(width, min.width, max.width);
  height = clamp(height, min.height, max.height);

  if (geometry_.width() != width || geometry_.height() != height) {
    OnResize(width, height);
  }
}

void AbstractView::Update() {
  OnUpdate(this);
}

bool AbstractView::Contain(int x, int y) const {
  return geometry_.Contain(x, y);
}

bool AbstractView::IsExpandX() const {
  return false;
}

bool AbstractView::IsExpandY() const {
  return false;
}

AbstractView *AbstractView::GetChildAt(int index) const {
  if (index < 0) index = children_count_ + index;

  AbstractView *object = nullptr;
  int mid = (children_count_ + 1) / 2;

  if (index >= 0 && index < mid) {
    object = first_child_;
    while (index > 0) {
      object = object->next_;
      index--;
    }
  } else if (index >= mid && index < children_count_) {
    object = last_child_;
    int max = children_count_ - 1;
    while (index < max) {
      object = object->previous_;
      index++;
    }
  }

  return object;
}

void AbstractView::PushFrontChild(AbstractView *child) {
  if (child->parent_) {
    if (child->parent_ == this)
      return;
    child->parent_->RemoveChild(child);
  }

  DBG_ASSERT(child->previous_ == nullptr);
  DBG_ASSERT(child->next_ == nullptr);
  DBG_ASSERT(child->parent_ == nullptr);

  if (first_child_) {
    first_child_->previous_ = child;
    child->next_ = first_child_;
  } else {
    DBG_ASSERT(last_child_ == nullptr);
    DBG_ASSERT(children_count_ == 0);
    // child->next_ = nullptr;
    last_child_ = child;
  }
  first_child_ = child;

  // child->previous_ = nullptr;
  child->parent_ = this;
  children_count_++;

  child->OnAddedToParent();
}

void AbstractView::InsertChild(AbstractView *child, int index) {
  if (child->parent_) {
    if (child->parent_ == this)
      return;
    child->parent_->RemoveChild(child);
  }

  DBG_ASSERT(child->previous_ == nullptr);
  DBG_ASSERT(child->next_ == nullptr);
  DBG_ASSERT(child->parent_ == nullptr);

  if (first_child_ == nullptr) {
    DBG_ASSERT(last_child_ == nullptr);
    DBG_ASSERT(children_count_ == 0);
    // child->next_ = nullptr;
    // child->previous_ = nullptr;
    last_child_ = child;
    first_child_ = child;
  } else {
    if (index >= 0) {
      AbstractView *p = first_child_;
      while (p && (index > 0)) {
        p = p->next_;
        index--;
      }
      if (p) {  // insert before p
        child->previous_ = p->previous_;
        child->next_ = p;
        if (p->previous_) p->previous_->next_ = child;
        else first_child_ = child;
        p->previous_ = child;
      } else {  // push back
        last_child_->next_ = child;
        child->previous_ = last_child_;
        last_child_ = child;
        // child->next_ = nullptr;
      }
    } else {
      AbstractView *p = last_child_;
      while (p && (index < -1)) {
        p = p->previous_;
        index++;
      }
      if (p) {  // insert after p
        child->next_ = p->next_;
        child->previous_ = p;
        if (p->next_) p->next_->previous_ = child;
        else last_child_ = child;
        p->next_ = child;
      } else {  // push front
        first_child_->previous_ = child;
        child->next_ = first_child_;
        first_child_ = child;
        // child->previous_ = nullptr;
      }
    }
  }

  child->parent_ = this;
  children_count_++;

  child->OnAddedToParent();
}

void AbstractView::PushBackChild(AbstractView *child) {
  if (child->parent_) {
    if (child->parent_ == this)
      return;
    child->parent_->RemoveChild(child);
  }

  DBG_ASSERT(child->previous_ == nullptr);
  DBG_ASSERT(child->next_ == nullptr);
  DBG_ASSERT(child->parent_ == nullptr);

  if (last_child_) {
    last_child_->next_ = child;
    child->previous_ = last_child_;
  } else {
    DBG_ASSERT(first_child_ == nullptr);
    DBG_ASSERT(children_count_ == 0);
    // child->previous_ = nullptr;
    first_child_ = child;
  }
  last_child_ = child;

  // child->next_ = nullptr;
  child->parent_ = this;
  children_count_++;

  child->OnAddedToParent();
}

AbstractView *AbstractView::RemoveChild(AbstractView *child) {
  if (child->parent_ != this) return nullptr;

  DBG_ASSERT(children_count_ > 0);

  if (child->previous_) {
    child->previous_->next_ = child->next_;
  } else {
    DBG_ASSERT(first_child_ == child);
    first_child_ = child->next_;
  }

  if (child->next_) {
    child->next_->previous_ = child->previous_;
  } else {
    DBG_ASSERT(last_child_ == child);
    last_child_ = child->previous_;
  }

  children_count_--;
  DBG_ASSERT(children_count_ >= 0);

  child->previous_ = nullptr;
  child->next_ = nullptr;
  child->parent_ = nullptr;

  child->OnRemovedFromParent(this);

  return child;
}

void AbstractView::ClearChildren() {
  AbstractView *ptr = first_child_;
  AbstractView *next_ptr = nullptr;

  while (ptr) {
    next_ptr = ptr->next_;
    // ptr->previous_ = nullptr;
    // ptr->next_ = nullptr;
    // ptr->parent_ = nullptr;
    delete ptr;
    ptr = next_ptr;
  }

  children_count_ = 0;
  first_child_ = nullptr;
  last_child_ = nullptr;
}

void AbstractView::OnAddedToParent() {
  // override in subclass
}

void AbstractView::OnRemovedFromParent(AbstractView *original_parent) {
  // override in subclass
}

bool AbstractView::SwapIndex(AbstractView *object1, AbstractView *object2) {
  if (object1 == nullptr || object2 == nullptr) return false;
  if (object1 == object2) return false;
  if (object1->parent_ != object2->parent_) return false;
  if (object1->parent_ == nullptr) return false;

  AbstractView *tmp1 = nullptr;
  AbstractView *tmp2 = nullptr;

  if (object1->next_ == object2) {    // view1 is just the previous sibling of view2

    DBG_ASSERT(object2->previous_ == object1);
    tmp1 = object1->previous_;
    tmp2 = object2->next_;
    object2->previous_ = tmp1;
    object1->previous_ = object2;
    object2->next_ = object1;
    object1->next_ = tmp2;

    if (tmp1 != nullptr) tmp1->next_ = object2;
    else object1->parent_->first_child_ = object2;

    if (tmp2 != nullptr) tmp2->previous_ = object1;
    else object2->parent_->last_child_ = object2;

  } else if (object1->previous_ == object2) {
    DBG_ASSERT(object2->next_ == object1);

    tmp1 = object2->previous_;
    tmp2 = object1->next_;
    object1->previous_ = tmp1;
    object2->previous_ = object1;
    object1->next_ = object2;
    object2->next_ = tmp2;

    if (tmp1 != nullptr) tmp1->next_ = object1;
    else object2->parent_->first_child_ = object1;

    if (tmp2 != nullptr) tmp2->previous_ = object2;
    else object1->parent_->last_child_ = object2;

  } else {

    tmp1 = object1->previous_;
    tmp2 = object2->previous_;
    object1->previous_ = tmp2;
    object2->previous_ = tmp1;

    if (tmp1 != nullptr) tmp1->next_ = object2;
    else object1->parent_->first_child_ = object2;

    if (tmp2 != nullptr) tmp2->next_ = object1;
    else object2->parent_->first_child_ = object1;

    tmp1 = object1->next_;
    tmp2 = object2->next_;
    object1->next_ = tmp2;
    object2->next_ = tmp1;

    if (tmp1 != nullptr) tmp1->previous_ = object2;
    else object1->parent_->last_child_ = object2;

    if (tmp2 != nullptr) tmp2->previous_ = object1;
    else object2->parent_->last_child_ = object1;

  }

  return true;
}

bool AbstractView::InsertSiblingBefore(AbstractView *src, AbstractView *dst) {
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->parent_ != nullptr) {

    if (dst->parent_ == src->parent_) {

      if (src->previous_ == dst) {    // already is the previous one of src
        return true;
      }

      if (dst->previous_) {
        dst->previous_->next_ = dst->next_;
      } else {
        DBG_ASSERT(dst->parent_->first_child_ == dst);
        dst->parent_->first_child_ = dst->next_;
      }

      if (dst->next_) {
        dst->next_->previous_ = dst->previous_;
      } else {
        DBG_ASSERT(dst->parent_->last_child_ == dst);
        dst->parent_->last_child_ = dst->previous_;
      }

      AbstractView *tmp = src->previous_;

      src->previous_ = dst;
      dst->next_ = src;
      dst->previous_ = tmp;

      if (tmp) {
        tmp->next_ = dst;
      } else {
        DBG_ASSERT(src->parent_->first_child_ == src);
        dst->parent_->first_child_ = dst;
      }

      return true;

    } else {
      dst->parent_->RemoveChild(dst);
    }

  }
  DBG_ASSERT(dst->parent_ == nullptr);
  DBG_ASSERT(dst->next_ == nullptr);
  DBG_ASSERT(dst->previous_ == nullptr);

  AbstractView *tmp = src->previous_;

  src->previous_ = dst;
  dst->next_ = src;
  dst->previous_ = tmp;
  if (tmp) {
    tmp->next_ = dst;
  } else {
    DBG_ASSERT(src->parent_->first_child_ == src);
    src->parent_->first_child_ = dst;
  }

  dst->parent_ = src->parent_;
  src->parent_->children_count_++;

  return true;
}

bool AbstractView::InsertSiblingAfter(AbstractView *src, AbstractView *dst) {
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->parent_ != nullptr) {

    if (dst->previous_ == src->parent_) {

      if (src->next_ == dst) {    // alrady is the next one of src
        return true;
      }

      if (dst->previous_) {
        dst->previous_->next_ = dst->next_;
      } else {
        DBG_ASSERT(dst->parent_->first_child_ == dst);
        dst->parent_->first_child_ = dst->next_;
      }

      if (dst->next_) {
        dst->next_->previous_ = dst->previous_;
      } else {
        DBG_ASSERT(dst->parent_->last_child_ == dst);
        dst->parent_->last_child_ = dst->previous_;
      }

      AbstractView *tmp = src->next_;

      src->next_ = dst;
      dst->previous_ = src;
      dst->next_ = tmp;

      if (tmp) {
        tmp->previous_ = dst;
      } else {
        DBG_ASSERT(src->parent_->last_child_ == src);
        dst->parent_->last_child_ = dst;
      }

      return true;

    } else {
      dst->parent_->RemoveChild(dst);
    }

  }

  DBG_ASSERT(dst->parent_ == nullptr);
  DBG_ASSERT(dst->next_ == nullptr);
  DBG_ASSERT(dst->previous_ == nullptr);

  AbstractView *tmp = src->next_;

  src->next_ = dst;
  dst->previous_ = src;
  dst->next_ = tmp;
  if (tmp) {
    tmp->previous_ = dst;
  } else {
    DBG_ASSERT(src->parent_->last_child_ == src);
    src->parent_->last_child_ = dst;
  }

  dst->parent_ = src->parent_;
  src->parent_->children_count_++;

  return true;
}

void AbstractView::MoveToFirst(AbstractView *object) {
  if (object->parent_) {

    if (object->parent_->first_child_ == object) {
      DBG_ASSERT(object->previous_ == 0);
      return;    // already at first
    }

    object->previous_->next_ = object->next_;
    if (object->next_) {
      object->next_->previous_ = object->previous_;
    } else {
      DBG_ASSERT(object->parent_->last_child_ == object);
      object->parent_->last_child_ = object->previous_;
    }

    object->previous_ = 0;
    object->next_ = object->parent_->first_child_;
    object->parent_->first_child_->previous_ = object;
    object->parent_->first_child_ = object;
  }
}

void AbstractView::MoveToLast(AbstractView *object) {
  if (object->parent_) {

    if (object->parent_->last_child_ == object) {
      DBG_ASSERT(object->next_ == 0);
      return;    // already at last
    }

    object->next_->previous_ = object->previous_;

    if (object->previous_) {
      object->previous_->next_ = object->next_;
    } else {
      DBG_ASSERT(object->parent_->first_child_ == object);
      object->parent_->first_child_ = object->next_;
    }

    object->next_ = 0;
    object->previous_ = object->parent_->last_child_;
    object->parent_->last_child_->next_ = object;
    object->parent_->last_child_ = object;
  }
}

void AbstractView::MoveForward(AbstractView *object) {
  if (object->parent_) {

    if (object->next_) {

      AbstractView *tmp = object->next_;

      tmp->previous_ = object->previous_;
      if (object->previous_) {
        object->previous_->next_ = tmp;
      } else {
        DBG_ASSERT(object->parent_->first_child_ == object);
        object->parent_->first_child_ = tmp;
      }

      object->previous_ = tmp;
      object->next_ = tmp->next_;
      if (tmp->next_) {
        tmp->next_->previous_ = object;
      }
      tmp->next_ = object;

      if (object->next_ == 0) {
        DBG_ASSERT(object->parent_->last_child_ == tmp);
        object->parent_->last_child_ = object;
      }

      if (object->previous_) {
        DBG_ASSERT(object->previous_->next_ == object);
      }
      if (object->next_) {
        DBG_ASSERT(object->next_->previous_ == object);
      }

    } else {
      DBG_ASSERT(object->parent_->last_child_ == object);
    }

  }
}

void AbstractView::MoveBackward(AbstractView *object) {
  if (object->parent_) {

    if (object->previous_) {

      AbstractView *tmp = object->previous_;

      tmp->next_ = object->next_;
      if (object->next_) {
        object->next_->previous_ = tmp;
      } else {
        DBG_ASSERT(object->parent_->last_child_ == object);
        object->parent_->last_child_ = tmp;
      }

      object->next_ = tmp;
      object->previous_ = tmp->previous_;
      if (tmp->previous_) {
        tmp->previous_->next_ = object;
      }
      tmp->previous_ = object;

      if (object->previous_ == 0) {
        DBG_ASSERT(object->parent_->first_child_ == tmp);
        object->parent_->first_child_ = object;
      }

      if (object->previous_) {
        DBG_ASSERT(object->previous_->next_ == object);
      }
      if (object->next_) {
        DBG_ASSERT(object->next_->previous_ == object);
      }

    } else {
      DBG_ASSERT(object->parent_->first_child_ == object);
    }

  }
}

void AbstractView::UpdateAll() {
  Update();
  for (AbstractView *sub = last_child_; sub; sub = sub->previous_) {
    sub->UpdateAll();
  }
}

void AbstractView::OnUpdate(AbstractView *view) {
  if (data_->redraw_task.IsLinked() && (view != this)) {
    // This view is going to be redrawn, just push back the task of the sub view

    data_->redraw_task.PushBack(&view->data_->redraw_task);
    view->data_->redraw_task.context = data_->redraw_task.context;
    return;
  }

  if (parent())
    parent_->OnUpdate(view);
}

Surface *AbstractView::OnGetSurface(const AbstractView *view) const {
  if (view->parent_)
    return view->parent_->OnGetSurface(view);

  return nullptr;
}

void AbstractView::TrackMouseMotion(MouseEvent *event) {
  if (data_->mouse_motion_task.IsLinked()) return;

  AbstractView *window = event->surface()->view();

  ViewTask *task = &window->data_->mouse_motion_task;
  while (task->next()) {
    task = static_cast<ViewTask *>(task->next());
  }
  task->PushBack(&data_->mouse_motion_task);
}

void AbstractView::UntrackMouseMotion() {
  data_->mouse_motion_task.Unlink();
}

Surface *AbstractView::GetSurface(const AbstractView *view) {
  return view->OnGetSurface(view);
}

void AbstractView::Damage(AbstractView *view, int surface_x, int surface_y, int width, int height) {
  view->data_->is_damaged_ = true;
  view->data_->damaged_region_.l = surface_x;
  view->data_->damaged_region_.t = surface_y;
  view->data_->damaged_region_.Resize(width, height);
}

void AbstractView::InitializeRedrawTaskList() {
  kRedrawTaskHead.PushBack(&kRedrawTaskTail);
}

void AbstractView::ClearRedrawTaskList() {
  Task *task = kRedrawTaskHead.next();
  Task *next_task = nullptr;
  while (task != &kRedrawTaskTail) {
    next_task = task->next();
    task->Unlink();
    task = next_task;
  }
}

}
