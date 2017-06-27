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

#include "internal/abstract-view_private.hpp"

#include "skland/numerical/bit.hpp"
#include "skland/numerical/clamp.hpp"

#include <skland/core/assert.hpp>
#include <skland/core/debug.hpp>

#include <skland/gui/abstract-shell-view.hpp>
#include <skland/gui/abstract-layout.hpp>
#include <skland/gui/mouse-event.hpp>

#include "internal/abstract-view_iterators.hpp"

namespace skland {
namespace gui {

using core::Point2I;
using core::SizeI;
using core::RectF;
using numerical::Clamp;
using numerical::Bit;

AbstractView::AbstractView()
    : AbstractView(400, 300) {

}

AbstractView::AbstractView(int width, int height)
    : AbstractEventHandler() {
  p_.reset(new Private(this));
  p_->geometry.Resize(width, height);
  p_->last_geometry.Resize(width, height);

  p_->preferred_size.width = Clamp(width, p_->minimal_size.width, p_->maximal_size.width);
  p_->preferred_size.height = Clamp(height, p_->minimal_size.height, p_->maximal_size.height);
}

AbstractView::~AbstractView() {
  _ASSERT(nullptr == p_->parent);
  _ASSERT(nullptr == p_->shell);
  _ASSERT(nullptr == p_->previous);
  _ASSERT(nullptr == p_->next);
  _ASSERT(0 == p_->children_count);
  _ASSERT(nullptr == p_->first_child);
  _ASSERT(nullptr == p_->last_child);

  // layout assert:
  _ASSERT(nullptr == p_->layout);
}

void AbstractView::SetMinimalWidth(int width) {
  if (width > p_->maximal_size.width) return;

  if (p_->preferred_size.width < width) p_->preferred_size.width = width;

  p_->minimal_size.width = width;

  if (p_->layout) {
    // TODO: relayout
  }
}

void AbstractView::SetMinimalHeight(int height) {
  if (height > p_->maximal_size.height) return;

  if (p_->preferred_size.height < height) p_->preferred_size.height = height;

  p_->minimal_size.height = height;

  if (p_->layout) {
    // TODO: relayout
  }
}

int AbstractView::GetMinimalWidth() const {
  return p_->minimal_size.width;
}

int AbstractView::GetMinimalHeight() const {
  return p_->minimal_size.height;
}

void AbstractView::SetPreferredWidth(int width) {
  if (width < p_->minimal_size.width || width > p_->maximal_size.width) return;

  p_->preferred_size.width = width;

  if (p_->layout) {
    // TODO: relayout
  }
}

void AbstractView::SetPreferredHeight(int height) {
  if (height < p_->minimal_size.height || height > p_->maximal_size.height) return;

  p_->preferred_size.height = height;

  if (p_->layout) {
    // TODO: relayout
  }
}

int AbstractView::GetPreferredWidth() const {
  return p_->preferred_size.width;
}

int AbstractView::GetPreferredHeight() const {
  return p_->preferred_size.height;
}

void AbstractView::SetMaximalWidth(int width) {
  if (width < p_->minimal_size.width) return;

  if (p_->preferred_size.width > width) p_->preferred_size.width = width;

  p_->maximal_size.width = width;

  if (p_->layout) {
    // TODO: relayout
  }
}

void AbstractView::SetMaximalHeight(int height) {
  if (height < p_->minimal_size.height) return;

  if (p_->preferred_size.height > height) p_->preferred_size.height = height;

  p_->maximal_size.height = height;

  if (p_->layout) {
    // TODO: relayout
  }
}

int AbstractView::GetMaximalWidth() const {
  return p_->maximal_size.width;
}

int AbstractView::GetMaximalHeight() const {
  return p_->maximal_size.height;
}

void AbstractView::SetLayoutPolicyOnX(LayoutPolicy policy) {
  p_->x_layout_policy = policy;

  if (p_->layout) {
    // TODO: relayout
  }
}

LayoutPolicy AbstractView::GetLayoutPolicyOnX() const {
  return p_->x_layout_policy;
}

void AbstractView::SetLayoutPolicyOnY(LayoutPolicy policy) {
  p_->y_layout_policy = policy;

  if (p_->layout) {
    // TODO: relayout
  }
}

LayoutPolicy AbstractView::GetLayoutPolicyOnY() const {
  return p_->y_layout_policy;
}

void AbstractView::MoveTo(int x, int y) {
  _ASSERT(!p_->is_drawing);

  if (p_->geometry.x() == x && p_->geometry.y() == y) return;

  p_->geometry.MoveTo(x, y);

  if (x == p_->last_geometry.x() && y == p_->last_geometry.y()) {
    Bit::Clear<int>(p_->dirty_flag, Private::kDirtyLeftMask | Private::kDirtyTopMask);
  } else {
    Bit::Set<int>(p_->dirty_flag, Private::kDirtyLeftMask | Private::kDirtyTopMask);
  }

  OnConfigureGeometry(p_->dirty_flag, p_->last_geometry, p_->geometry);
}

const core::Padding &AbstractView::GetPadding() const {
  return p_->padding;
}

void AbstractView::Resize(int width, int height) {
  _ASSERT(!p_->is_drawing);

  if (p_->geometry.width() == width && p_->geometry.height() == height) return;

  SizeI min;
  min.width = GetMinimalWidth();
  min.height = GetMinimalHeight();
  SizeI max;
  max.width = GetMaximalWidth();
  max.height = GetMaximalHeight();

  if (min.width > max.width || min.height > max.height) {
    throw std::runtime_error("Error! Invalid minimal and maximal size!");
  }

  if (width < min.width || height < min.height) return;
  if (width > max.width || height > max.height) return;

  p_->geometry.Resize(width, height);

  if (width == p_->last_geometry.width() && height == p_->last_geometry.height()) {
    Bit::Clear<int>(p_->dirty_flag, Private::kDirtyRightMask | Private::kDirtyBottomMask);
  } else {
    Bit::Set<int>(p_->dirty_flag, Private::kDirtyRightMask | Private::kDirtyBottomMask);
  }

  OnConfigureGeometry(p_->dirty_flag, p_->last_geometry, p_->geometry);
}

int AbstractView::GetX() const {
  return static_cast<int>(p_->geometry.x());
}

int AbstractView::GetY() const {
  return static_cast<int>(p_->geometry.y());
}

int AbstractView::GetLeft() const {
  if (p_->parent)
    return static_cast<int>(p_->geometry.left - p_->parent->p_->geometry.left);

  return static_cast<int>(p_->geometry.left);
}

void AbstractView::SetLeft(int left) {
  if (p_->parent) left += p_->parent->p_->geometry.left;

  if (left == p_->geometry.left) return;

  int width = static_cast<int>(p_->geometry.right) - left;
  if (width < p_->minimal_size.width || width > p_->maximal_size.width) return;

  p_->geometry.left = left;

  if (p_->geometry.left == p_->last_geometry.left)
    Bit::Clear<int>(p_->dirty_flag, Private::kDirtyLeftMask | Private::kDirtyWidthMask);
  else
    Bit::Set<int>(p_->dirty_flag, Private::kDirtyLeftMask | Private::kDirtyWidthMask);

  OnConfigureGeometry(p_->dirty_flag, p_->last_geometry, p_->geometry);
}

int AbstractView::GetTop() const {
  if (p_->parent)
    return static_cast<int>(p_->geometry.top - p_->parent->p_->geometry.top);

  return static_cast<int>(p_->geometry.top);
}

void AbstractView::SetTop(int top) {
  if (p_->parent) top += p_->parent->p_->geometry.top;

  if (top == p_->geometry.top) return;

  int height = static_cast<int>(p_->geometry.bottom) - top;
  if (height < p_->minimal_size.height || height > p_->maximal_size.height) return;

  p_->geometry.top = top;

  if (p_->geometry.top == p_->last_geometry.top)
    Bit::Clear<int>(p_->dirty_flag, Private::kDirtyTopMask | Private::kDirtyHeightMask);
  else
    Bit::Set<int>(p_->dirty_flag, Private::kDirtyTopMask | Private::kDirtyHeightMask);

  OnConfigureGeometry(p_->dirty_flag, p_->last_geometry, p_->geometry);
}

int AbstractView::GetRight() const {
  return GetLeft() + static_cast<int>(p_->geometry.width());
}

void AbstractView::SetRight(int right) {
  if (p_->parent) right += p_->parent->p_->geometry.left; // relative to parent

  if (right == p_->geometry.right) return;

  int width = right - static_cast<int>(p_->geometry.left);
  if (width < p_->minimal_size.width || width > p_->maximal_size.width) return;

  p_->geometry.right = right;

  if (p_->geometry.right == p_->last_geometry.right)
    Bit::Clear<int>(p_->dirty_flag, Private::kDirtyRightMask | Private::kDirtyWidthMask);
  else
    Bit::Set<int>(p_->dirty_flag, Private::kDirtyRightMask | Private::kDirtyWidthMask);

  OnConfigureGeometry(p_->dirty_flag, p_->last_geometry, p_->geometry);
}

int AbstractView::GetBottom() const {
  return GetTop() + static_cast<int>(p_->geometry.height());
}

void AbstractView::SetBottom(int bottom) {
  if (p_->parent) bottom += p_->parent->p_->geometry.top; // relative to parent

  if (bottom == p_->geometry.bottom) return;

  int height = bottom - static_cast<int>(p_->geometry.top);
  if (height < p_->minimal_size.height || height > p_->maximal_size.height) return;

  p_->geometry.bottom = bottom;

  if (p_->geometry.bottom == p_->last_geometry.bottom)
    Bit::Clear<int>(p_->dirty_flag, Private::kDirtyBottomMask | Private::kDirtyHeightMask);
  else
    Bit::Set<int>(p_->dirty_flag, Private::kDirtyBottomMask | Private::kDirtyHeightMask);

  OnConfigureGeometry(p_->dirty_flag, p_->last_geometry, p_->geometry);
}

int AbstractView::GetWidth() const {
  return static_cast<int>(p_->geometry.width());
}

int AbstractView::GetHeight() const {
  return static_cast<int>(p_->geometry.height());
}

float AbstractView::GetXCenter() const {
  return p_->geometry.center_x();
}

float AbstractView::GetYCenter() const {
  return p_->geometry.center_y();
}

const RectF &AbstractView::GetGeometry() const {
  return p_->geometry;
}

bool AbstractView::IsVisible() const {
  return p_->visible;
}

void AbstractView::AddAnchorTo(AbstractView *target, Alignment align, int distance) {
  if (target == p_->parent || this == target->p_->parent) {
    switch (align) {
      case kAlignLeft: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->left_anchor_group.PushBack(pair.first);
        target->p_->left_anchor_group.PushBack(pair.second);
        break;
      }
      case kAlignTop: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->top_anchor_group.PushBack(pair.first);
        target->p_->top_anchor_group.PushBack(pair.second);
        break;
      }
      case kAlignRight: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->right_anchor_group.PushBack(pair.first);
        target->p_->right_anchor_group.PushBack(pair.second);
        break;
      }
      case kAlignBottom: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->bottom_anchor_group.PushBack(pair.first);
        target->p_->bottom_anchor_group.PushBack(pair.second);
        break;
      }
      default:break;
    }
  } else if (target->p_->parent == p_->parent) {
    switch (align) {
      case kAlignLeft: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->right_anchor_group.PushBack(pair.first);
        target->p_->left_anchor_group.PushBack(pair.second);
        break;
      }
      case kAlignTop: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->bottom_anchor_group.PushBack(pair.first);
        target->p_->top_anchor_group.PushBack(pair.second);
        break;
      }
      case kAlignRight: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->left_anchor_group.PushBack(pair.first);
        target->p_->right_anchor_group.PushBack(pair.second);
        break;
      }
      case kAlignBottom: {
        std::pair<Anchor *, Anchor *> pair = Anchor::MakePair(distance, this, target);
        p_->top_anchor_group.PushBack(pair.first);
        target->p_->bottom_anchor_group.PushBack(pair.second);
        break;
      }
      default:break;
    }
  } else {
    _DEBUG("%s\n", "Error! Cannot add anchor to the view which have no relationship!");
  }
}

const AnchorGroup &AbstractView::GetAnchorGroup(Alignment align) const {
  switch (align) {
    case kAlignLeft:return p_->left_anchor_group;
    case kAlignTop:return p_->top_anchor_group;
    case kAlignRight:return p_->right_anchor_group;
    case kAlignBottom:return p_->bottom_anchor_group;
    default:return p_->left_anchor_group;
  }
}

void AbstractView::Update() {
  if (p_->is_drawing) return;

  if (p_->redraw_task.IsLinked()) return;

  OnUpdate(this);
}

void AbstractView::CancelUpdate() {
  if (p_->is_drawing) return;

  p_->redraw_task.Unlink();
}

bool AbstractView::Contain(int x, int y) const {
  return p_->geometry.Contain(x, y);
}

void AbstractView::Destroy() {
  destroyed_.Emit(this);

  OnDestroy();

  if (p_->layout) {
    _ASSERT(p_->layout == p_->parent);
    p_->layout->RemoveView(this);
  }

  if (p_->parent) {
    _ASSERT(nullptr == p_->shell);
    p_->parent->RemoveChild(this);
  } else if (p_->shell) {
    _ASSERT(nullptr == p_->parent);
    p_->shell->DetachView(this);
  }

  if (p_->children_count > 0) {
    _ASSERT(p_->first_child);
    _ASSERT(p_->last_child);
    ClearChildren();
  }

  delete this;
}

AbstractView *AbstractView::GetParent() const {
  return p_->parent;
}

AbstractLayout *AbstractView::GetLayout() const {
  return p_->layout;
}

void AbstractView::OnChildAdded(AbstractView */*view*/) {
  // override in subclass
}

void AbstractView::OnChildRemoved(AbstractView */*view*/) {

}

void AbstractView::OnAddedToParent() {
  // override in subclass
}

void AbstractView::OnRemovedFromParent(AbstractView */*original_parent*/) {
  // override in subclass
}

void AbstractView::OnAttachedToShellView() {

}

void AbstractView::OnDetachedFromShellView(AbstractShellView */*shell_view*/) {

}

void AbstractView::DispatchUpdate() {
  for (AbstractView *sub = p_->last_child; sub; sub = sub->p_->previous) {
    sub->Update();
    sub->DispatchUpdate();
  }
}

void AbstractView::OnUpdate(AbstractView *view) {
  if (p_->redraw_task.IsLinked()) {
    if (view != this) {
      // This view is going to be redrawn, just push back the task of the sub view
      _ASSERT(p_->redraw_task.context.canvas());
      p_->redraw_task.PushBack(&view->p_->redraw_task);
      view->p_->redraw_task.context = p_->redraw_task.context;
    }
    return;
  }

  if (p_->parent) {
    _ASSERT(nullptr == p_->shell);
    p_->parent->OnUpdate(view);
    return;
  } else if (p_->shell) {
    _ASSERT(nullptr == p_->parent);
    p_->shell->OnUpdate(view);
  }
}

Surface *AbstractView::GetSurface(const AbstractView *view) const {
  if (p_->parent) {
    _ASSERT(nullptr == p_->shell);
    return p_->parent->GetSurface(view);
  }

  if (p_->shell) {
    _ASSERT(nullptr == p_->parent);
    return p_->shell->GetSurface(view);
  }

  return nullptr;
}

void AbstractView::OnEnterOutput(const Surface *surface, const Output *output) {
  // override this in sub class
}

void AbstractView::OnLeaveOutput(const Surface *surface, const Output *output) {
  // override this in sub class
}

AbstractView *AbstractView::DispatchMouseEnterEvent(MouseEvent *event) {
  Iterator it(this);
  AbstractView *view = nullptr;
  Point2I cursor_xy(event->GetWindowXY());

  for (it = it.first_child(); it; ++it) {
    if (it.view()->Contain(cursor_xy.x, cursor_xy.y)) {
      view = it.view();
      break;
    }
  }

  return view;
}

void AbstractView::OnDestroy() {
  // override in sub class
}

void AbstractView::TrackMouseMotion(MouseEvent *event) {
//  if (p_->mouse_motion_task.IsLinked()) return;

//  AbstractView *window = event->surface()->event_handler();
//
//  ViewTask *task = &window->p_->mouse_motion_task;
//  while (task->next()) {
//    task = static_cast<ViewTask *>(task->next());
//  }
//  task->PushBack(&p_->mouse_motion_task);
}

void AbstractView::UntrackMouseMotion() {
//  p_->mouse_motion_task.Unlink();
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
  if (child->p_->parent == this) {
    _ASSERT(nullptr == child->p_->shell);
    return;
  }

  if (child->p_->parent) {
    _ASSERT(nullptr == child->p_->shell);
    child->p_->parent->RemoveChild(child);
  } else if (child->p_->shell) {
    _ASSERT(nullptr == child->p_->parent);
    child->p_->shell->DetachView(child);
  }

  _ASSERT(nullptr == child->p_->previous);
  _ASSERT(nullptr == child->p_->next);
  _ASSERT(nullptr == child->p_->parent);

  if (p_->first_child) {
    p_->first_child->p_->previous = child;
    child->p_->next = p_->first_child;
  } else {
    _ASSERT(nullptr == p_->last_child);
    _ASSERT(0 == p_->children_count);
    // child->data_->next_ = nullptr;
    p_->last_child = child;
  }
  p_->first_child = child;

  // child->data_->previous_ = nullptr;
  child->p_->parent = this;
  p_->children_count++;

  OnChildAdded(child);
  if (child->p_->parent == this)
    child->OnAddedToParent();
}

void AbstractView::InsertChild(AbstractView *child, int index) {
  if (child->p_->parent == this) {
    _ASSERT(nullptr == child->p_->shell);
    return;
  }

  if (child->p_->parent) {
    _ASSERT(nullptr == child->p_->shell);
    child->p_->parent->RemoveChild(child);
  } else if (child->p_->shell) {
    _ASSERT(nullptr == child->p_->parent);
    child->p_->shell->DetachView(child);
  }

  _ASSERT(nullptr == child->p_->previous);
  _ASSERT(nullptr == child->p_->next);
  _ASSERT(nullptr == child->p_->parent);

  if (nullptr == p_->first_child) {
    _ASSERT(nullptr == p_->last_child);
    _ASSERT(0 == p_->children_count);
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

  OnChildAdded(child);
  if (child->p_->parent == this)
    child->OnAddedToParent();
}

void AbstractView::PushBackChild(AbstractView *child) {
  if (child->p_->parent == this) {
    _ASSERT(nullptr == child->p_->shell);
    return;
  }

  if (child->p_->parent) {
    _ASSERT(nullptr == child->p_->shell);
    child->p_->parent->RemoveChild(child);
  } else if (child->p_->shell) {
    _ASSERT(nullptr == child->p_->parent);
    child->p_->shell->DetachView(child);
  }

  _ASSERT(child->p_->previous == nullptr);
  _ASSERT(child->p_->next == nullptr);
  _ASSERT(child->p_->parent == nullptr);

  if (p_->last_child) {
    p_->last_child->p_->next = child;
    child->p_->previous = p_->last_child;
  } else {
    _ASSERT(p_->first_child == nullptr);
    _ASSERT(p_->children_count == 0);
    // child->data_->previous_ = nullptr;
    p_->first_child = child;
  }
  p_->last_child = child;

  // child->data_->next_ = nullptr;
  child->p_->parent = this;
  p_->children_count++;

  OnChildAdded(child);
  if (child->p_->parent == this)
    child->OnAddedToParent();
}

AbstractView *AbstractView::RemoveChild(AbstractView *child) {
  if (child->p_->parent != this) return nullptr;

  _ASSERT(p_->children_count > 0);

  if (child->p_->previous) {
    child->p_->previous->p_->next = child->p_->next;
  } else {
    _ASSERT(p_->first_child == child);
    p_->first_child = child->p_->next;
  }

  if (child->p_->next) {
    child->p_->next->p_->previous = child->p_->previous;
  } else {
    _ASSERT(p_->last_child == child);
    p_->last_child = child->p_->previous;
  }

  p_->children_count--;
  _ASSERT(p_->children_count >= 0);

  child->p_->previous = nullptr;
  child->p_->next = nullptr;
  child->p_->parent = nullptr;

  OnChildRemoved(child);
  if (child->p_->parent != this)
    child->OnRemovedFromParent(this);

  return child;
}

void AbstractView::ClearChildren() {
  AbstractView *ptr = p_->first_child;
  AbstractView *next = nullptr;

  while (ptr) {
    next = ptr->p_->next;
    // ptr->previous_ = nullptr;
    // ptr->next_ = nullptr;
    // ptr->parent_ = nullptr;
    ptr->Destroy();
    ptr = next;
  }

  p_->children_count = 0;
  p_->first_child = nullptr;
  p_->last_child = nullptr;
}

bool AbstractView::SwapIndex(AbstractView *object1, AbstractView *object2) {
  if (object1 == nullptr || object2 == nullptr) return false;
  if (object1 == object2) return false;
  if (object1->p_->parent != object2->p_->parent) return false;
  if (object1->p_->parent == nullptr) return false;

  AbstractView *tmp1 = nullptr;
  AbstractView *tmp2 = nullptr;

  if (object1->p_->next == object2) {    // view1 is just the previous sibling of view2

    _ASSERT(object2->p_->previous == object1);
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
    _ASSERT(object2->p_->next == object1);

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
        _ASSERT(dst->p_->parent->p_->first_child == dst);
        dst->p_->parent->p_->first_child = dst->p_->next;
      }

      if (dst->p_->next) {
        dst->p_->next->p_->previous = dst->p_->previous;
      } else {
        _ASSERT(dst->p_->parent->p_->last_child == dst);
        dst->p_->parent->p_->last_child = dst->p_->previous;
      }

      AbstractView *tmp = src->p_->previous;

      src->p_->previous = dst;
      dst->p_->next = src;
      dst->p_->previous = tmp;

      if (tmp) {
        tmp->p_->next = dst;
      } else {
        _ASSERT(src->p_->parent->p_->first_child == src);
        dst->p_->parent->p_->first_child = dst;
      }

      return true;

    } else {
      dst->p_->parent->RemoveChild(dst);
    }

  }
  _ASSERT(dst->p_->parent == nullptr);
  _ASSERT(dst->p_->next == nullptr);
  _ASSERT(dst->p_->previous == nullptr);

  AbstractView *tmp = src->p_->previous;

  src->p_->previous = dst;
  dst->p_->next = src;
  dst->p_->previous = tmp;
  if (tmp) {
    tmp->p_->next = dst;
  } else {
    _ASSERT(src->p_->parent->p_->first_child == src);
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
        _ASSERT(dst->p_->parent->p_->first_child == dst);
        dst->p_->parent->p_->first_child = dst->p_->next;
      }

      if (dst->p_->next) {
        dst->p_->next->p_->previous = dst->p_->previous;
      } else {
        _ASSERT(dst->p_->parent->p_->last_child == dst);
        dst->p_->parent->p_->last_child = dst->p_->previous;
      }

      AbstractView *tmp = src->p_->next;

      src->p_->next = dst;
      dst->p_->previous = src;
      dst->p_->next = tmp;

      if (tmp) {
        tmp->p_->previous = dst;
      } else {
        _ASSERT(src->p_->parent->p_->last_child == src);
        dst->p_->parent->p_->last_child = dst;
      }

      return true;

    } else {
      dst->p_->parent->RemoveChild(dst);
    }

  }

  _ASSERT(dst->p_->parent == nullptr);
  _ASSERT(dst->p_->next == nullptr);
  _ASSERT(dst->p_->previous == nullptr);

  AbstractView *tmp = src->p_->next;

  src->p_->next = dst;
  dst->p_->previous = src;
  dst->p_->next = tmp;
  if (tmp) {
    tmp->p_->previous = dst;
  } else {
    _ASSERT(src->p_->parent->p_->last_child == src);
    src->p_->parent->p_->last_child = dst;
  }

  dst->p_->parent = src->p_->parent;
  src->p_->parent->p_->children_count++;

  return true;
}

void AbstractView::MoveToFirst(AbstractView *view) {
  if (view->p_->parent) {

    if (view->p_->parent->p_->first_child == view) {
      _ASSERT(view->p_->previous == 0);
      return;    // already at first
    }

    view->p_->previous->p_->next = view->p_->next;
    if (view->p_->next) {
      view->p_->next->p_->previous = view->p_->previous;
    } else {
      _ASSERT(view->p_->parent->p_->last_child == view);
      view->p_->parent->p_->last_child = view->p_->previous;
    }

    view->p_->previous = 0;
    view->p_->next = view->p_->parent->p_->first_child;
    view->p_->parent->p_->first_child->p_->previous = view;
    view->p_->parent->p_->first_child = view;
  }
}

void AbstractView::MoveToLast(AbstractView *view) {
  if (view->p_->parent) {

    if (view->p_->parent->p_->last_child == view) {
      _ASSERT(view->p_->next == 0);
      return;    // already at last
    }

    view->p_->next->p_->previous = view->p_->previous;

    if (view->p_->previous) {
      view->p_->previous->p_->next = view->p_->next;
    } else {
      _ASSERT(view->p_->parent->p_->first_child == view);
      view->p_->parent->p_->first_child = view->p_->next;
    }

    view->p_->next = 0;
    view->p_->previous = view->p_->parent->p_->last_child;
    view->p_->parent->p_->last_child->p_->next = view;
    view->p_->parent->p_->last_child = view;
  }
}

void AbstractView::MoveForward(AbstractView *view) {
  if (view->p_->parent) {

    if (view->p_->next) {

      AbstractView *tmp = view->p_->next;

      tmp->p_->previous = view->p_->previous;
      if (view->p_->previous) {
        view->p_->previous->p_->next = tmp;
      } else {
        _ASSERT(view->p_->parent->p_->first_child == view);
        view->p_->parent->p_->first_child = tmp;
      }

      view->p_->previous = tmp;
      view->p_->next = tmp->p_->next;
      if (tmp->p_->next) {
        tmp->p_->next->p_->previous = view;
      }
      tmp->p_->next = view;

      if (view->p_->next == 0) {
        _ASSERT(view->p_->parent->p_->last_child == tmp);
        view->p_->parent->p_->last_child = view;
      }

      if (view->p_->previous) {
        _ASSERT(view->p_->previous->p_->next == view);
      }
      if (view->p_->next) {
        _ASSERT(view->p_->next->p_->previous == view);
      }

    } else {
      _ASSERT(view->p_->parent->p_->last_child == view);
    }

  }
}

void AbstractView::MoveBackward(AbstractView *view) {
  if (view->p_->parent) {

    if (view->p_->previous) {

      AbstractView *tmp = view->p_->previous;

      tmp->p_->next = view->p_->next;
      if (view->p_->next) {
        view->p_->next->p_->previous = tmp;
      } else {
        _ASSERT(view->p_->parent->p_->last_child == view);
        view->p_->parent->p_->last_child = tmp;
      }

      view->p_->next = tmp;
      view->p_->previous = tmp->p_->previous;
      if (tmp->p_->previous) {
        tmp->p_->previous->p_->next = view;
      }
      tmp->p_->previous = view;

      if (view->p_->previous == 0) {
        _ASSERT(view->p_->parent->p_->first_child == tmp);
        view->p_->parent->p_->first_child = view;
      }

      if (view->p_->previous) {
        _ASSERT(view->p_->previous->p_->next == view);
      }
      if (view->p_->next) {
        _ASSERT(view->p_->next->p_->previous == view);
      }

    } else {
      _ASSERT(view->p_->parent->p_->first_child == view);
    }

  }
}

void AbstractView::Damage(AbstractView *view, int surface_x, int surface_y, int width, int height) {
  view->p_->is_damaged = true;
  view->p_->damaged_region.l = surface_x;
  view->p_->damaged_region.t = surface_y;
  view->p_->damaged_region.Resize(width, height);
}

// -------------------

void AbstractView::RedrawTask::Run() const {
  view_->p_->is_drawing = true;

  if (view_->p_->dirty_flag) {
    view_->OnGeometryChange(view_->p_->dirty_flag, view_->p_->last_geometry, view_->p_->geometry);
    view_->p_->last_geometry = view_->p_->geometry;
  }

  if (view_->p_->need_layout) {
    view_->p_->is_layouting = true;
    view_->OnLayout(view_->p_->dirty_flag,
                   view_->p_->padding.left,
                   view_->p_->padding.top,
                   static_cast<int>(view_->p_->geometry.width()) - view_->p_->padding.right,
                   static_cast<int>(view_->p_->geometry.height()) - view_->p_->padding.bottom);
    view_->p_->is_layouting = false;
  }

  if (view_->p_->need_redraw)   // This boolean flag is reset in AbstractLayout
    view_->OnDraw(&context);

  if (view_->p_->is_damaged) {
    context.surface()->Damage(view_->p_->damaged_region.x(),
                              view_->p_->damaged_region.y(),
                              view_->p_->damaged_region.width(),
                              view_->p_->damaged_region.height());
  }

  view_->p_->dirty_flag = 0;
  view_->p_->need_layout = false;
  view_->p_->is_damaged = false;
  view_->p_->visible = true;

  view_->p_->is_drawing = false;
}

AbstractView::RedrawTask *AbstractView::RedrawTask::Get(const AbstractView *view) {
  return &view->p_->redraw_task;
}

} // namespace gui
} // namespace skland
