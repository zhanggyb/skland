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

#include <skland/gui/surface.hpp>

#include <skland/gui/abstract-event-handler.hpp>
#include <skland/gui/buffer.hpp>

#include <skland/gui/output.hpp>

#include "internal/display-registry.hpp"
#include "internal/surface-commit-task.hpp"

namespace skland {

Surface *Surface::Shell::Create(AbstractEventHandler *event_handler, const Margin &margin) {
  Surface *surface = new Surface(event_handler, margin);
  surface->role_.shell = new Shell(surface);
  return surface;
}

Surface::Shell *Surface::Shell::Get(const Surface *surface) {
  if (nullptr == surface->parent_)
    return surface->role_.shell;

  return nullptr;
}

void Surface::Shell::ResizeWindow(int width, int height) const {
  xdg_surface_.SetWindowGeometry(surface_->margin_.l,
                                 surface_->margin_.t,
                                 width, height);
}

Surface::Shell::Shell(Surface *surface)
    : surface_(surface), parent_(nullptr) {
  DBG_ASSERT(surface_);
  role_.placeholder = nullptr;
  xdg_surface_.Setup(Display::Registry().xdg_shell(), surface_->wl_surface_);

  Push();
}

Surface::Shell::~Shell() {
  Remove();

  if (nullptr == parent_) delete role_.toplevel;
  else delete role_.popup;

  xdg_surface_.Destroy();

  DBG_ASSERT(surface_->role_.shell == this);
  surface_->role_.shell = nullptr;
}

void Surface::Shell::Push() {
  DBG_ASSERT(nullptr == surface_->parent_);
  DBG_ASSERT(nullptr == surface_->up_);
  DBG_ASSERT(nullptr == surface_->down_);

  DBG_ASSERT(Surface::kShellSurfaceCount >= 0);

  if (Surface::kTop) {
    Surface::kTop->up_ = surface_;
    surface_->down_ = Surface::kTop;
    Surface::kTop = surface_;
  } else {
    DBG_ASSERT(Surface::kShellSurfaceCount == 0);
    DBG_ASSERT(nullptr == Surface::kBottom);
    Surface::kBottom = surface_;
    Surface::kTop = surface_;
  }

  Surface::kShellSurfaceCount++;
}

void Surface::Shell::Remove() {
  DBG_ASSERT(nullptr == surface_->parent_);

  if (surface_->up_) {
    surface_->up_->down_ = surface_->down_;
  } else {
    DBG_ASSERT(Surface::kTop == surface_);
    Surface::kTop = surface_->down_;
  }

  if (surface_->down_) {
    surface_->down_->up_ = surface_->up_;
  } else {
    DBG_ASSERT(Surface::kBottom == surface_);
    Surface::kBottom = surface_->up_;
  }

  surface_->up_ = nullptr;
  surface_->down_ = nullptr;
  Surface::kShellSurfaceCount--;
  DBG_ASSERT(Surface::kShellSurfaceCount >= 0);
}

// ------

Surface *Surface::Shell::Toplevel::Create(AbstractEventHandler *event_handler, const Margin &margin) {
  Surface *surface = Shell::Create(event_handler, margin);
  Shell *shell = Shell::Get(surface);
  shell->role_.toplevel = new Toplevel(shell);
  return surface;
}

Surface::Shell::Toplevel *Surface::Shell::Toplevel::Get(const Surface *surface) {
  Shell *shell = Shell::Get(surface);
  if (nullptr == shell || shell->parent_) return nullptr;
  return shell->role_.toplevel;
}

void Surface::Shell::Toplevel::SetTitle(const char *title) const {
  xdg_toplevel_.SetTitle(title);
}

void Surface::Shell::Toplevel::SetAppId(const char *id) const {
  xdg_toplevel_.SetAppId(id);
}

void Surface::Shell::Toplevel::Move(const wayland::Seat &seat, uint32_t serial) const {
  xdg_toplevel_.Move(seat, serial);
}

void Surface::Shell::Toplevel::Resize(const wayland::Seat &seat, uint32_t serial, uint32_t edges) const {
  xdg_toplevel_.Resize(seat, serial, edges);
}

void Surface::Shell::Toplevel::SetMaximized() const {
  xdg_toplevel_.SetMaximized();
}

void Surface::Shell::Toplevel::UnsetMaximized() const {
  xdg_toplevel_.UnsetMaximized();
}

void Surface::Shell::Toplevel::SetFullscreen(const Output &output) const {
  xdg_toplevel_.SetFullscreen(output.GetOutput());
}

void Surface::Shell::Toplevel::UnsetFullscreen(const Output &output) const {
  xdg_toplevel_.UnsetFullscreen(output.GetOutput());
}

void Surface::Shell::Toplevel::SetMinimized() const {
  xdg_toplevel_.SetMinimized();
}

Surface::Shell::Toplevel::Toplevel(Shell *shell_surface) {
  DBG_ASSERT(shell_surface);
  shell_ = shell_surface;
  xdg_toplevel_.Setup(shell_->xdg_surface_);
}

Surface::Shell::Toplevel::~Toplevel() {
  xdg_toplevel_.Destroy();

  DBG_ASSERT(shell_->role_.toplevel == this);
  DBG_ASSERT(nullptr == shell_->parent_);
  shell_->role_.toplevel = nullptr;
}

// ------

Surface *Surface::Shell::Popup::Create(Shell *parent, AbstractEventHandler *view, const Margin &margin) {
  Surface *surface = Shell::Create(view, margin);
  Shell *shell = Shell::Get(surface);
  shell->parent_ = parent;
  shell->role_.popup = new Popup(shell);
  return surface;
}

Surface::Shell::Popup::Popup(Shell *shell)
    : shell_(shell) {
  // TODO: initialize xdg_popup_
}

Surface::Shell::Popup::~Popup() {
  xdg_popup_.Destroy();

  DBG_ASSERT(shell_->parent_);
  DBG_ASSERT(shell_->role_.popup == this);

  shell_->parent_ = nullptr;
  shell_->role_.popup = nullptr;
}

// ------

Surface *Surface::Sub::Create(Surface *parent, AbstractEventHandler *event_handler, const Margin &margin) {
  Surface *surface = new Surface(event_handler, margin);
  surface->role_.sub = new Sub(surface, parent);
  return surface;
}

Surface::Sub *Surface::Sub::Get(const Surface *surface) {
  if (nullptr == surface->parent_)
    return nullptr;

  return surface->role_.sub;
}

Surface::Sub::Sub(Surface *surface, Surface *parent)
    : surface_(surface) {
  DBG_ASSERT(surface_);
  DBG_ASSERT(parent);
  wl_sub_surface_.Setup(Display::Registry().wl_subcompositor(),
                        surface_->wl_surface_,
                        parent->wl_surface_);
  SetParent(parent);
}

Surface::Sub::~Sub() {
  DBG_ASSERT(surface_->role_.sub == this);

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
  surface_->role_.sub = nullptr;
}

void Surface::Sub::PlaceAbove(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->parent() == sibling->parent() ||
      surface_ == sibling->parent() ||
      surface_->parent() == sibling) {
    wl_sub_surface_.PlaceAbove(sibling->wl_surface_);
    MoveAbove(sibling);
  }
}

void Surface::Sub::PlaceBelow(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->parent() == sibling->parent() ||
      surface_ == sibling->parent() ||
      surface_->parent() == sibling) {
    wl_sub_surface_.PlaceBelow(sibling->wl_surface_);
    MoveBelow(sibling);
  }
}

void Surface::Sub::SetRelativePosition(int x, int y) {
  wl_sub_surface_.SetPosition(x, y);
  surface_->relative_position_.x = x;
  surface_->relative_position_.y = y;
}

void Surface::Sub::SetWindowPosition(int x, int y) {
  Point parent_global_position = surface_->parent()->GetWindowPosition();
  int local_x = x - parent_global_position.x;
  int local_y = y - parent_global_position.y;
  wl_sub_surface_.SetPosition(local_x, local_y);
  surface_->relative_position_.x = x;
  surface_->relative_position_.y = y;
}

void Surface::Sub::SetParent(Surface *parent) {
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

void Surface::Sub::MoveAbove(Surface *dst) {
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

void Surface::Sub::MoveBelow(Surface *dst) {
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

void Surface::Sub::InsertAbove(Surface *sibling) {
  DBG_ASSERT(surface_->parent_ == sibling->parent_ ||
      surface_ == sibling->parent_ ||
      surface_->parent_ == sibling);
  if (sibling->above_) sibling->above_->below_ = surface_;
  surface_->above_ = sibling->above_;
  sibling->above_ = surface_;
  surface_->below_ = sibling;
}

void Surface::Sub::InsertBelow(Surface *sibling) {
  DBG_ASSERT(surface_->parent_ == sibling->parent_ ||
      surface_ == sibling->parent_ ||
      surface_->parent_ == sibling);
  if (sibling->below_) sibling->below_->above_ = surface_;
  surface_->below_ = sibling->below_;
  sibling->below_ = surface_;
  surface_->above_ = sibling;
}

// ------

Surface::EGL *Surface::EGL::Get(Surface *surface) {
  if (nullptr == surface->egl_)
    surface->egl_ = new EGL(surface);

  return surface->egl_;
}

Surface::EGL::EGL(Surface *surface)
    : surface_(surface) {
  egl_surface_.Setup(Display::Registry().egl_display(),
                     surface_->wl_surface_, 400, 400);
//                     surface->event_handler()->width(),
//                     surface->event_handler()->height());
}

Surface::EGL::~EGL() {
  egl_surface_.Destroy();
  surface_->egl_ = nullptr;
}

bool Surface::EGL::MakeCurrent() {
  return Display::Registry().egl_display().MakeCurrent(egl_surface_, egl_surface_);
}

bool Surface::EGL::SwapBuffers() {
  return Display::Registry().egl_display().SwapBuffers(egl_surface_);
}

bool Surface::EGL::SwapBuffersWithDamage(int x, int y, int width, int height) {
  return Display::Registry().egl_display().SwapBuffersWithDamage(egl_surface_, x, y, width, height);
}

bool Surface::EGL::SwapInterval(EGLint interval) {
  return Display::Registry().egl_display().SwapInterval(interval);
}

// ------

Surface *Surface::kTop = nullptr;
Surface *Surface::kBottom = nullptr;
int Surface::kShellSurfaceCount = 0;
Task Surface::kCommitTaskHead;
Task Surface::kCommitTaskTail;

Surface::Surface(AbstractEventHandler *event_handler, const Margin &margin)
    : mode_(kSynchronized),
      parent_(nullptr),
      above_(nullptr),
      below_(nullptr),
      up_(nullptr),
      down_(nullptr),
      event_handler_(event_handler),
      margin_(margin),
      buffer_transform_(WL_OUTPUT_TRANSFORM_NORMAL),
      buffer_scale_(1),
      is_user_data_set_(false),
      egl_(nullptr) {
  DBG_ASSERT(nullptr != event_handler_);
  role_.placeholder = nullptr;

  wl_surface_.enter().Set(this, &Surface::OnEnter);
  wl_surface_.leave().Set(this, &Surface::OnLeave);
  wl_surface_.Setup(Display::Registry().wl_compositor());

  commit_task_.reset(new CommitTask(this));
}

Surface::~Surface() {
  if (egl_)
    delete egl_;

  if (nullptr == parent_)
    delete role_.shell; // deleting a shell surface will break the links to up_ and down_
  else
    delete role_.sub; // deleting all sub surfaces and break the links to above_ and below_
}

void Surface::Attach(Buffer *buffer, int32_t x, int32_t y) {
  if (nullptr != egl_) return;

  if (nullptr == buffer) {
    wl_surface_.Attach(NULL, x, y);
  } else {
    buffer->SetPosition(x, y);
    wl_surface_.Attach(buffer->wl_buffer(), x, y);
  }
}

void Surface::Commit() {
  if (nullptr != egl_) {
    // EGL surface does not use commit
    if (mode_ == kSynchronized) {
      // Synchronized mode need to commit the main surface
      Surface *main_surface = GetShellSurface();
      main_surface->Commit();
    }

    return;
  }

  if (commit_task_->IsLinked())
    return;

  if (nullptr == parent_) {
    kCommitTaskTail.PushFront(commit_task_.get());
    return;
  }

  if (mode_ == kSynchronized) {
    // Synchronized mode need to commit the main surface too
    Surface *main_surface = GetShellSurface();
    main_surface->Commit();
    main_surface->commit_task_->PushFront(commit_task_.get());
  } else {
    kCommitTaskTail.PushFront(commit_task_.get());
  }
}

Surface *Surface::GetShellSurface() {
  Surface *shell_surface = this;
  Surface *parent = parent_;

  while (parent) {
    shell_surface = parent;
    parent = parent->parent_;
  }

  return shell_surface;
}

Point Surface::GetWindowPosition() const {
  Point position = relative_position_;

  const Surface *parent = parent_;
  const Surface *shell_surface = this;

  while (parent) {
    position += parent->relative_position();
    if (nullptr == parent->parent_) shell_surface = parent;
    parent = parent->parent();
  }

  return position - Point(shell_surface->margin().l, shell_surface->margin().t);
}

void Surface::OnEnter(struct wl_output *wl_output) {
  if (!is_user_data_set_) {
    wl_surface_.SetUserData(this);
    is_user_data_set_ = true;
  }

  Output *output = static_cast<Output *>(wl_output_get_user_data(wl_output));
  event_handler_->OnEnterOutput(output);
}

void Surface::OnLeave(struct wl_output *wl_output) {
  Output *output = static_cast<Output *>(wl_output_get_user_data(wl_output));
  event_handler_->OnEnterOutput(output);
}

void Surface::Clear() {
  while (kShellSurfaceCount > 0) {
    AbstractEventHandler *event_handler = kTop->event_handler();
    delete event_handler;
  }
}

void Surface::InitializeCommitTaskList() {
  kCommitTaskHead.PushBack(&kCommitTaskTail);
}

void Surface::ClearCommitTaskList() {
  Task *task = kCommitTaskHead.next();
  Task *next_task = nullptr;
  while (task != &kCommitTaskTail) {
    next_task = task->next();
    task->Unlink();
    task = next_task;
  }
}

}
