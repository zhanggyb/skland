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
      geometry_(width, height) {
  p_.reset(new Private(this));
}

AbstractView::~AbstractView() {
  if (p_->parent) p_->parent->RemoveChild(this);
  DBG_ASSERT(p_->previous == nullptr);
  DBG_ASSERT(p_->next == nullptr);

  if (p_->children_count > 0) {
    DBG_ASSERT(p_->first_child != nullptr);
    DBG_ASSERT(p_->last_child != nullptr);
    ClearChildren();
  }
  DBG_ASSERT(p_->children_count == 0);
  DBG_ASSERT(p_->first_child == nullptr);
  DBG_ASSERT(p_->last_child == nullptr);
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
  if (index < 0) index = p_->children_count + index;

  AbstractView *object = nullptr;
  int mid = (p_->children_count + 1) / 2;

  if (index >= 0 && index < mid) {
    object = p_->first_child;
    while (index > 0) {
      object = object->p_->next;
      index--;
    }
  } else if (index >= mid && index < p_->children_count) {
    object = p_->last_child;
    int max = p_->children_count - 1;
    while (index < max) {
      object = object->p_->previous;
      index++;
    }
  }

  return object;
}

void AbstractView::PushFrontChild(AbstractView *child) {
  if (child->p_->parent) {
    if (child->p_->parent == this)
      return;
    child->p_->parent->RemoveChild(child);
  }

  DBG_ASSERT(child->p_->previous == nullptr);
  DBG_ASSERT(child->p_->next == nullptr);
  DBG_ASSERT(child->p_->parent == nullptr);

  if (p_->first_child) {
    p_->first_child->p_->previous = child;
    child->p_->next = p_->first_child;
  } else {
    DBG_ASSERT(p_->last_child == nullptr);
    DBG_ASSERT(p_->children_count == 0);
    // child->data_->next_ = nullptr;
    p_->last_child = child;
  }
  p_->first_child = child;

  // child->data_->previous_ = nullptr;
  child->p_->parent = this;
  p_->children_count++;

  child->OnAddedToParent();
}

void AbstractView::InsertChild(AbstractView *child, int index) {
  if (child->p_->parent) {
    if (child->p_->parent == this)
      return;
    child->p_->parent->RemoveChild(child);
  }

  DBG_ASSERT(child->p_->previous == nullptr);
  DBG_ASSERT(child->p_->next == nullptr);
  DBG_ASSERT(child->p_->parent == nullptr);

  if (p_->first_child == nullptr) {
    DBG_ASSERT(p_->last_child == nullptr);
    DBG_ASSERT(p_->children_count == 0);
    // child->data_->next_ = nullptr;
    // child->data_->previous_ = nullptr;
    p_->last_child = child;
    p_->first_child = child;
  } else {
    if (index >= 0) {
      AbstractView *p = p_->first_child;
      while (p && (index > 0)) {
        p = p->p_->next;
        index--;
      }
      if (p) {  // insert before p
        child->p_->previous = p->p_->previous;
        child->p_->next = p;
        if (p->p_->previous) p->p_->previous->p_->next = child;
        else p_->first_child = child;
        p->p_->previous = child;
      } else {  // push back
        p_->last_child->p_->next = child;
        child->p_->previous = p_->last_child;
        p_->last_child = child;
        // child->data_->next_ = nullptr;
      }
    } else {
      AbstractView *p = p_->last_child;
      while (p && (index < -1)) {
        p = p->p_->previous;
        index++;
      }
      if (p) {  // insert after p
        child->p_->next = p->p_->next;
        child->p_->previous = p;
        if (p->p_->next) p->p_->next->p_->previous = child;
        else p_->last_child = child;
        p->p_->next = child;
      } else {  // push front
        p_->first_child->p_->previous = child;
        child->p_->next = p_->first_child;
        p_->first_child = child;
        // child->data_->previous_ = nullptr;
      }
    }
  }

  child->p_->parent = this;
  p_->children_count++;

  child->OnAddedToParent();
}

void AbstractView::PushBackChild(AbstractView *child) {
  if (child->p_->parent) {
    if (child->p_->parent == this)
      return;
    child->p_->parent->RemoveChild(child);
  }

  DBG_ASSERT(child->p_->previous == nullptr);
  DBG_ASSERT(child->p_->next == nullptr);
  DBG_ASSERT(child->p_->parent == nullptr);

  if (p_->last_child) {
    p_->last_child->p_->next = child;
    child->p_->previous = p_->last_child;
  } else {
    DBG_ASSERT(p_->first_child == nullptr);
    DBG_ASSERT(p_->children_count == 0);
    // child->data_->previous_ = nullptr;
    p_->first_child = child;
  }
  p_->last_child = child;

  // child->data_->next_ = nullptr;
  child->p_->parent = this;
  p_->children_count++;

  child->OnAddedToParent();
}

AbstractView *AbstractView::RemoveChild(AbstractView *child) {
  if (child->p_->parent != this) return nullptr;

  DBG_ASSERT(p_->children_count > 0);

  if (child->p_->previous) {
    child->p_->previous->p_->next = child->p_->next;
  } else {
    DBG_ASSERT(p_->first_child == child);
    p_->first_child = child->p_->next;
  }

  if (child->p_->next) {
    child->p_->next->p_->previous = child->p_->previous;
  } else {
    DBG_ASSERT(p_->last_child == child);
    p_->last_child = child->p_->previous;
  }

  p_->children_count--;
  DBG_ASSERT(p_->children_count >= 0);

  child->p_->previous = nullptr;
  child->p_->next = nullptr;
  child->p_->parent = nullptr;

  child->OnRemovedFromParent(this);

  return child;
}

void AbstractView::ClearChildren() {
  AbstractView *ptr = p_->first_child;
  AbstractView *next_ptr = nullptr;

  while (ptr) {
    next_ptr = ptr->p_->next;
    // ptr->previous_ = nullptr;
    // ptr->next_ = nullptr;
    // ptr->parent_ = nullptr;
    delete ptr;
    ptr = next_ptr;
  }

  p_->children_count = 0;
  p_->first_child = nullptr;
  p_->last_child = nullptr;
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
  if (object1->p_->parent != object2->p_->parent) return false;
  if (object1->p_->parent == nullptr) return false;

  AbstractView *tmp1 = nullptr;
  AbstractView *tmp2 = nullptr;

  if (object1->p_->next == object2) {    // view1 is just the previous sibling of view2

    DBG_ASSERT(object2->p_->previous == object1);
    tmp1 = object1->p_->previous;
    tmp2 = object2->p_->next;
    object2->p_->previous = tmp1;
    object1->p_->previous = object2;
    object2->p_->next = object1;
    object1->p_->next = tmp2;

    if (tmp1 != nullptr) tmp1->p_->next = object2;
    else object1->p_->parent->p_->first_child = object2;

    if (tmp2 != nullptr) tmp2->p_->previous = object1;
    else object2->p_->parent->p_->last_child = object2;

  } else if (object1->p_->previous == object2) {
    DBG_ASSERT(object2->p_->next == object1);

    tmp1 = object2->p_->previous;
    tmp2 = object1->p_->next;
    object1->p_->previous = tmp1;
    object2->p_->previous = object1;
    object1->p_->next = object2;
    object2->p_->next = tmp2;

    if (tmp1 != nullptr) tmp1->p_->next = object1;
    else object2->p_->parent->p_->first_child = object1;

    if (tmp2 != nullptr) tmp2->p_->previous = object2;
    else object1->p_->parent->p_->last_child = object2;

  } else {

    tmp1 = object1->p_->previous;
    tmp2 = object2->p_->previous;
    object1->p_->previous = tmp2;
    object2->p_->previous = tmp1;

    if (tmp1 != nullptr) tmp1->p_->next = object2;
    else object1->p_->parent->p_->first_child = object2;

    if (tmp2 != nullptr) tmp2->p_->next = object1;
    else object2->p_->parent->p_->first_child = object1;

    tmp1 = object1->p_->next;
    tmp2 = object2->p_->next;
    object1->p_->next = tmp2;
    object2->p_->next = tmp1;

    if (tmp1 != nullptr) tmp1->p_->previous = object2;
    else object1->p_->parent->p_->last_child = object2;

    if (tmp2 != nullptr) tmp2->p_->previous = object1;
    else object2->p_->parent->p_->last_child = object1;

  }

  return true;
}

bool AbstractView::InsertSiblingBefore(AbstractView *src, AbstractView *dst) {
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->p_->parent != nullptr) {

    if (dst->p_->parent == src->p_->parent) {

      if (src->p_->previous == dst) {    // already is the previous one of src
        return true;
      }

      if (dst->p_->previous) {
        dst->p_->previous->p_->next = dst->p_->next;
      } else {
        DBG_ASSERT(dst->p_->parent->p_->first_child == dst);
        dst->p_->parent->p_->first_child = dst->p_->next;
      }

      if (dst->p_->next) {
        dst->p_->next->p_->previous = dst->p_->previous;
      } else {
        DBG_ASSERT(dst->p_->parent->p_->last_child == dst);
        dst->p_->parent->p_->last_child = dst->p_->previous;
      }

      AbstractView *tmp = src->p_->previous;

      src->p_->previous = dst;
      dst->p_->next = src;
      dst->p_->previous = tmp;

      if (tmp) {
        tmp->p_->next = dst;
      } else {
        DBG_ASSERT(src->p_->parent->p_->first_child == src);
        dst->p_->parent->p_->first_child = dst;
      }

      return true;

    } else {
      dst->p_->parent->RemoveChild(dst);
    }

  }
  DBG_ASSERT(dst->p_->parent == nullptr);
  DBG_ASSERT(dst->p_->next == nullptr);
  DBG_ASSERT(dst->p_->previous == nullptr);

  AbstractView *tmp = src->p_->previous;

  src->p_->previous = dst;
  dst->p_->next = src;
  dst->p_->previous = tmp;
  if (tmp) {
    tmp->p_->next = dst;
  } else {
    DBG_ASSERT(src->p_->parent->p_->first_child == src);
    src->p_->parent->p_->first_child = dst;
  }

  dst->p_->parent = src->p_->parent;
  src->p_->parent->p_->children_count++;

  return true;
}

bool AbstractView::InsertSiblingAfter(AbstractView *src, AbstractView *dst) {
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->p_->parent != nullptr) {

    if (dst->p_->previous == src->p_->parent) {

      if (src->p_->next == dst) {    // alrady is the next one of src
        return true;
      }

      if (dst->p_->previous) {
        dst->p_->previous->p_->next = dst->p_->next;
      } else {
        DBG_ASSERT(dst->p_->parent->p_->first_child == dst);
        dst->p_->parent->p_->first_child = dst->p_->next;
      }

      if (dst->p_->next) {
        dst->p_->next->p_->previous = dst->p_->previous;
      } else {
        DBG_ASSERT(dst->p_->parent->p_->last_child == dst);
        dst->p_->parent->p_->last_child = dst->p_->previous;
      }

      AbstractView *tmp = src->p_->next;

      src->p_->next = dst;
      dst->p_->previous = src;
      dst->p_->next = tmp;

      if (tmp) {
        tmp->p_->previous = dst;
      } else {
        DBG_ASSERT(src->p_->parent->p_->last_child == src);
        dst->p_->parent->p_->last_child = dst;
      }

      return true;

    } else {
      dst->p_->parent->RemoveChild(dst);
    }

  }

  DBG_ASSERT(dst->p_->parent == nullptr);
  DBG_ASSERT(dst->p_->next == nullptr);
  DBG_ASSERT(dst->p_->previous == nullptr);

  AbstractView *tmp = src->p_->next;

  src->p_->next = dst;
  dst->p_->previous = src;
  dst->p_->next = tmp;
  if (tmp) {
    tmp->p_->previous = dst;
  } else {
    DBG_ASSERT(src->p_->parent->p_->last_child == src);
    src->p_->parent->p_->last_child = dst;
  }

  dst->p_->parent = src->p_->parent;
  src->p_->parent->p_->children_count++;

  return true;
}

void AbstractView::MoveToFirst(AbstractView *object) {
  if (object->p_->parent) {

    if (object->p_->parent->p_->first_child == object) {
      DBG_ASSERT(object->p_->previous == 0);
      return;    // already at first
    }

    object->p_->previous->p_->next = object->p_->next;
    if (object->p_->next) {
      object->p_->next->p_->previous = object->p_->previous;
    } else {
      DBG_ASSERT(object->p_->parent->p_->last_child == object);
      object->p_->parent->p_->last_child = object->p_->previous;
    }

    object->p_->previous = 0;
    object->p_->next = object->p_->parent->p_->first_child;
    object->p_->parent->p_->first_child->p_->previous = object;
    object->p_->parent->p_->first_child = object;
  }
}

void AbstractView::MoveToLast(AbstractView *object) {
  if (object->p_->parent) {

    if (object->p_->parent->p_->last_child == object) {
      DBG_ASSERT(object->p_->next == 0);
      return;    // already at last
    }

    object->p_->next->p_->previous = object->p_->previous;

    if (object->p_->previous) {
      object->p_->previous->p_->next = object->p_->next;
    } else {
      DBG_ASSERT(object->p_->parent->p_->first_child == object);
      object->p_->parent->p_->first_child = object->p_->next;
    }

    object->p_->next = 0;
    object->p_->previous = object->p_->parent->p_->last_child;
    object->p_->parent->p_->last_child->p_->next = object;
    object->p_->parent->p_->last_child = object;
  }
}

void AbstractView::MoveForward(AbstractView *object) {
  if (object->p_->parent) {

    if (object->p_->next) {

      AbstractView *tmp = object->p_->next;

      tmp->p_->previous = object->p_->previous;
      if (object->p_->previous) {
        object->p_->previous->p_->next = tmp;
      } else {
        DBG_ASSERT(object->p_->parent->p_->first_child == object);
        object->p_->parent->p_->first_child = tmp;
      }

      object->p_->previous = tmp;
      object->p_->next = tmp->p_->next;
      if (tmp->p_->next) {
        tmp->p_->next->p_->previous = object;
      }
      tmp->p_->next = object;

      if (object->p_->next == 0) {
        DBG_ASSERT(object->p_->parent->p_->last_child == tmp);
        object->p_->parent->p_->last_child = object;
      }

      if (object->p_->previous) {
        DBG_ASSERT(object->p_->previous->p_->next == object);
      }
      if (object->p_->next) {
        DBG_ASSERT(object->p_->next->p_->previous == object);
      }

    } else {
      DBG_ASSERT(object->p_->parent->p_->last_child == object);
    }

  }
}

void AbstractView::MoveBackward(AbstractView *object) {
  if (object->p_->parent) {

    if (object->p_->previous) {

      AbstractView *tmp = object->p_->previous;

      tmp->p_->next = object->p_->next;
      if (object->p_->next) {
        object->p_->next->p_->previous = tmp;
      } else {
        DBG_ASSERT(object->p_->parent->p_->last_child == object);
        object->p_->parent->p_->last_child = tmp;
      }

      object->p_->next = tmp;
      object->p_->previous = tmp->p_->previous;
      if (tmp->p_->previous) {
        tmp->p_->previous->p_->next = object;
      }
      tmp->p_->previous = object;

      if (object->p_->previous == 0) {
        DBG_ASSERT(object->p_->parent->p_->first_child == tmp);
        object->p_->parent->p_->first_child = object;
      }

      if (object->p_->previous) {
        DBG_ASSERT(object->p_->previous->p_->next == object);
      }
      if (object->p_->next) {
        DBG_ASSERT(object->p_->next->p_->previous == object);
      }

    } else {
      DBG_ASSERT(object->p_->parent->p_->first_child == object);
    }

  }
}

void AbstractView::UpdateAll() {
  Update();
  for (AbstractView *sub = p_->last_child; sub; sub = sub->p_->previous) {
    sub->UpdateAll();
  }
}

void AbstractView::OnUpdate(AbstractView *view) {
  if (p_->redraw_task.IsLinked() && (view != this)) {
    // This view is going to be redrawn, just push back the task of the sub view

    p_->redraw_task.PushBack(&view->p_->redraw_task);
    view->p_->redraw_task.context = p_->redraw_task.context;
    return;
  }

  if (p_->parent)
    p_->parent->OnUpdate(view);
}

Surface *AbstractView::OnGetSurface(const AbstractView *view) const {
  if (view->p_->parent)
    return view->p_->parent->OnGetSurface(view);

  return nullptr;
}

void AbstractView::TrackMouseMotion(MouseEvent *event) {
  if (p_->mouse_motion_task.IsLinked()) return;

  AbstractView *window = event->surface()->view();

  ViewTask *task = &window->p_->mouse_motion_task;
  while (task->next()) {
    task = static_cast<ViewTask *>(task->next());
  }
  task->PushBack(&p_->mouse_motion_task);
}

void AbstractView::UntrackMouseMotion() {
  p_->mouse_motion_task.Unlink();
}

Surface *AbstractView::GetSurface(const AbstractView *view) {
  return view->OnGetSurface(view);
}

void AbstractView::Damage(AbstractView *view, int surface_x, int surface_y, int width, int height) {
  view->p_->is_damaged_ = true;
  view->p_->damaged_region_.l = surface_x;
  view->p_->damaged_region_.t = surface_y;
  view->p_->damaged_region_.Resize(width, height);
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
