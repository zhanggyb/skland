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

#include "internal/surface_shell_private.hpp"
#include "internal/surface_shell_toplevel_private.hpp"
#include "internal/surface_shell_popup_private.hpp"
#include "internal/surface_egl_private.hpp"
#include "internal/surface-commit_task.hpp"

#include <skland/gui/abstract-event-handler.hpp>
#include <skland/gui/buffer.hpp>

#include <skland/core/assert.hpp>
#include <skland/gui/input-event.hpp>
#include <skland/gui/region.hpp>

#include "internal/input_private.hpp"
#include "internal/output_private.hpp"
#include "internal/display_native.hpp"
#include "internal/buffer_private.hpp"

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
  zxdg_surface_v6_set_window_geometry(p_->zxdg_surface,
                                      surface_->margin_.l, surface_->margin_.t,
                                      width, height);
}

void Surface::Shell::AckConfigure(uint32_t serial) const {
  zxdg_surface_v6_ack_configure(p_->zxdg_surface, serial);
}

Surface::Shell::Shell(Surface *surface)
    : surface_(surface), parent_(nullptr) {
  p_.reset(new Private);

  _ASSERT(surface_);
  role_.placeholder = nullptr;

  p_->zxdg_surface = zxdg_shell_v6_get_xdg_surface(Display::Native().xdg_shell(), surface_->wl_surface_);
  zxdg_surface_v6_add_listener(p_->zxdg_surface, &Private::kListener, this);

  Push();
}

Surface::Shell::~Shell() {
  Remove();

  if (nullptr == parent_) delete role_.toplevel;
  else delete role_.popup;

  _ASSERT(surface_->role_.shell == this);
  surface_->role_.shell = nullptr;
}

void Surface::Shell::Push() {
  _ASSERT(nullptr == surface_->parent_);
  _ASSERT(nullptr == surface_->up_);
  _ASSERT(nullptr == surface_->down_);

  _ASSERT(Surface::kShellSurfaceCount >= 0);

  if (Surface::kTop) {
    Surface::kTop->up_ = surface_;
    surface_->down_ = Surface::kTop;
    Surface::kTop = surface_;
  } else {
    _ASSERT(Surface::kShellSurfaceCount == 0);
    _ASSERT(nullptr == Surface::kBottom);
    Surface::kBottom = surface_;
    Surface::kTop = surface_;
  }

  Surface::kShellSurfaceCount++;
}

void Surface::Shell::Remove() {
  _ASSERT(nullptr == surface_->parent_);

  if (surface_->up_) {
    surface_->up_->down_ = surface_->down_;
  } else {
    _ASSERT(Surface::kTop == surface_);
    Surface::kTop = surface_->down_;
  }

  if (surface_->down_) {
    surface_->down_->up_ = surface_->up_;
  } else {
    _ASSERT(Surface::kBottom == surface_);
    Surface::kBottom = surface_->up_;
  }

  surface_->up_ = nullptr;
  surface_->down_ = nullptr;
  Surface::kShellSurfaceCount--;
  _ASSERT(Surface::kShellSurfaceCount >= 0);
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
  zxdg_toplevel_v6_set_title(p_->zxdg_toplevel, title);
}

void Surface::Shell::Toplevel::SetAppId(const char *id) const {
  zxdg_toplevel_v6_set_app_id(p_->zxdg_toplevel, id);
}

void Surface::Shell::Toplevel::Move(const InputEvent &input_event, uint32_t serial) const {
  zxdg_toplevel_v6_move(p_->zxdg_toplevel, input_event.input_->p_->wl_seat, serial);
}

void Surface::Shell::Toplevel::Resize(const InputEvent &input_event, uint32_t serial, uint32_t edges) const {
  zxdg_toplevel_v6_resize(p_->zxdg_toplevel, input_event.input_->p_->wl_seat, serial, edges);
}

void Surface::Shell::Toplevel::SetMaximized() const {
  zxdg_toplevel_v6_set_maximized(p_->zxdg_toplevel);
}

void Surface::Shell::Toplevel::UnsetMaximized() const {
  zxdg_toplevel_v6_unset_maximized(p_->zxdg_toplevel);
}

void Surface::Shell::Toplevel::SetFullscreen(const Output *output) const {
  zxdg_toplevel_v6_set_fullscreen(p_->zxdg_toplevel, output->p_->wl_output);
}

void Surface::Shell::Toplevel::UnsetFullscreen() const {
  zxdg_toplevel_v6_unset_fullscreen(p_->zxdg_toplevel);
}

void Surface::Shell::Toplevel::SetMinimized() const {
  zxdg_toplevel_v6_set_minimized(p_->zxdg_toplevel);
}

Surface::Shell::Toplevel::Toplevel(Shell *shell_surface) {
  p_.reset(new Private);

  _ASSERT(shell_surface);
  shell_ = shell_surface;

  p_->zxdg_toplevel = zxdg_surface_v6_get_toplevel(shell_surface->p_->zxdg_surface);
  zxdg_toplevel_v6_add_listener(p_->zxdg_toplevel, &Private::kListener, this);
}

Surface::Shell::Toplevel::~Toplevel() {
  _ASSERT(shell_->role_.toplevel == this);
  _ASSERT(nullptr == shell_->parent_);
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
  p_.reset(new Private);
  // TODO: initialize xdg_popup_
}

Surface::Shell::Popup::~Popup() {

  _ASSERT(shell_->parent_);
  _ASSERT(shell_->role_.popup == this);

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
    : surface_(surface), wl_sub_surface_(nullptr) {
  _ASSERT(surface_);
  _ASSERT(parent);

  wl_sub_surface_ = wl_subcompositor_get_subsurface(Display::Native().wl_subcompositor(),
                                                    surface_->wl_surface_,
                                                    parent->wl_surface_);
  SetParent(parent);
}

Surface::Sub::~Sub() {
  _ASSERT(surface_->role_.sub == this);

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

  if (wl_sub_surface_)
    wl_subsurface_destroy(wl_sub_surface_);

  surface_->role_.sub = nullptr;
}

void Surface::Sub::PlaceAbove(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->parent() == sibling->parent() ||
      surface_ == sibling->parent() ||
      surface_->parent() == sibling) {
    wl_subsurface_place_above(wl_sub_surface_, sibling->wl_surface_);
    MoveAbove(sibling);
  }
}

void Surface::Sub::PlaceBelow(Surface *sibling) {
  if (sibling == surface_) return;

  if (surface_->parent() == sibling->parent() ||
      surface_ == sibling->parent() ||
      surface_->parent() == sibling) {
    wl_subsurface_place_below(wl_sub_surface_, sibling->wl_surface_);
    MoveBelow(sibling);
  }
}

void Surface::Sub::SetRelativePosition(int x, int y) {
  wl_subsurface_set_position(wl_sub_surface_, x, y);
  surface_->relative_position_.x = x;
  surface_->relative_position_.y = y;
}

void Surface::Sub::SetWindowPosition(int x, int y) {
  Point parent_global_position = surface_->parent()->GetWindowPosition();
  int local_x = x - parent_global_position.x;
  int local_y = y - parent_global_position.y;
  wl_subsurface_set_position(wl_sub_surface_, local_x, local_y);
  surface_->relative_position_.x = x;
  surface_->relative_position_.y = y;
}

void Surface::Sub::SetParent(Surface *parent) {
  _ASSERT(surface_->parent_ == nullptr &&
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
  _ASSERT(surface_->parent_ == sibling->parent_ ||
      surface_ == sibling->parent_ ||
      surface_->parent_ == sibling);
  if (sibling->above_) sibling->above_->below_ = surface_;
  surface_->above_ = sibling->above_;
  sibling->above_ = surface_;
  surface_->below_ = sibling;
}

void Surface::Sub::InsertBelow(Surface *sibling) {
  _ASSERT(surface_->parent_ == sibling->parent_ ||
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
  p_.reset(new Private);

  p_->wl_egl_window_ =
      wl_egl_window_create(surface_->wl_surface_, 400, 400);
  p_->egl_surface_ =
      eglCreatePlatformWindowSurface(Display::Native().egl_display(),
                                     Display::Native().egl_config(),
                                     p_->wl_egl_window_,
                                     NULL);
}

Surface::EGL::~EGL() {
  if (p_->egl_surface_) {
    _ASSERT(p_->wl_egl_window_);
    wl_egl_window_destroy(p_->wl_egl_window_);
  }
  surface_->egl_ = nullptr;
}

bool Surface::EGL::MakeCurrent() {
  return EGL_TRUE ==
      eglMakeCurrent(Display::Native().egl_display(),
                     p_->egl_surface_,
                     p_->egl_surface_,
                     Display::Native().egl_context());
}

bool Surface::EGL::SwapBuffers() {
  return EGL_TRUE ==
      eglSwapBuffers(Display::Native().egl_display(), p_->egl_surface_);
}

bool Surface::EGL::SwapBuffersWithDamage(int x, int y, int width, int height) {
  EGLint rect[4] = {x, y, width, height};
  return EGL_TRUE
      == Display::Native::kSwapBuffersWithDamageAPI(Display::Native().egl_display(),
                                                    p_->egl_surface_,
                                                    rect,
                                                    4 * sizeof(EGLint));
}

bool Surface::EGL::SwapInterval(EGLint interval) {
  return EGL_TRUE == eglSwapInterval(Display::Native().egl_display(), interval);
}

void Surface::EGL::Resize(int width, int height, int dx, int dy) {
  wl_egl_window_resize(p_->wl_egl_window_, width, height, dx, dy);
}

// ------

Surface *Surface::kTop = nullptr;
Surface *Surface::kBottom = nullptr;
int Surface::kShellSurfaceCount = 0;
Task Surface::kCommitTaskHead;
Task Surface::kCommitTaskTail;

const struct wl_surface_listener Surface::kListener = {
    OnEnter,
    OnLeave
};

void Surface::OnEnter(void *data, struct wl_surface *wl_surface, struct wl_output *wl_output) {
  const Surface *_this = static_cast<const Surface *>(data);
  const Output *output = static_cast<const Output *>(wl_output_get_user_data(wl_output));
  _this->event_handler_->OnEnterOutput(_this, output);
}

void Surface::OnLeave(void *data, struct wl_surface *wl_surface, struct wl_output *wl_output) {
  const Surface *_this = static_cast<const Surface *>(data);
  const Output *output = static_cast<const Output *>(wl_output_get_user_data(wl_output));
  _this->event_handler_->OnLeaveOutput(_this, output);
}

Surface::Surface(AbstractEventHandler *event_handler, const Margin &margin)
    : mode_(kSynchronized),
      parent_(nullptr),
      above_(nullptr),
      below_(nullptr),
      up_(nullptr),
      down_(nullptr),
      wl_surface_(nullptr),
      event_handler_(event_handler),
      margin_(margin),
      buffer_transform_(WL_OUTPUT_TRANSFORM_NORMAL),
      buffer_scale_(1),
      is_user_data_set_(false),
      egl_(nullptr) {
  _ASSERT(nullptr != event_handler_);
  role_.placeholder = nullptr;

  wl_surface_ = wl_compositor_create_surface(Display::Native().wl_compositor());
  wl_surface_add_listener(wl_surface_, &kListener, this);

  commit_task_.reset(new CommitTask(this));
}

Surface::~Surface() {
  if (egl_)
    delete egl_;

  if (nullptr == parent_)
    delete role_.shell; // deleting a shell surface will break the links to up_ and down_
  else
    delete role_.sub; // deleting all sub surfaces and break the links to above_ and below_

  if (wl_surface_)
    wl_surface_destroy(wl_surface_);
}

void Surface::Attach(Buffer *buffer, int32_t x, int32_t y) {
  if (nullptr != egl_) return;

  if (nullptr == buffer) {
    wl_surface_attach(wl_surface_, NULL, x, y);
  } else {
    buffer->SetPosition(x, y);
    wl_surface_attach(wl_surface_, buffer->p_->wl_buffer, x, y);
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

void Surface::Damage(int surface_x, int surface_y, int width, int height) const {
  wl_surface_damage(wl_surface_, surface_x, surface_y, width, height);
}

void Surface::SetInputRegion(const Region &region) {
  wl_surface_set_input_region(wl_surface_, region.wl_region_);
}

void Surface::SetOpaqueRegion(const Region &region) {
  wl_surface_set_opaque_region(wl_surface_, region.wl_region_);
}

void Surface::SetBufferTransform(int32_t transform) {
  wl_surface_set_buffer_transform(wl_surface_, transform);
}

void Surface::SetBufferScale(int32_t scale) {
  wl_surface_set_buffer_scale(wl_surface_, scale);
}

void Surface::DamageBuffer(int32_t x, int32_t y, int32_t width, int32_t height) {
  wl_surface_damage_buffer(wl_surface_, x, y, width, height);
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
