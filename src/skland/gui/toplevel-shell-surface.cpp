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

#include <skland/core/defines.hpp>

#include <skland/gui/toplevel-shell-surface.hpp>
#include <skland/gui/shell-surface.hpp>
#include <skland/gui/output.hpp>

namespace skland {

struct ToplevelShellSurface::Private {

  Private(const Private &) = delete;
  Private &operator=(const Private &) = delete;

  Private()
      : shell_surface_(nullptr) {}

  ~Private() {}

  ShellSurface *shell_surface_;
  wayland::XdgToplevel xdg_toplevel_;

};

Surface *ToplevelShellSurface::Create(AbstractEventHandler *event_handler, const Margin &margin) {
  Surface *surface = ShellSurface::Create(event_handler, margin);
  ShellSurface *shell_surface = ShellSurface::Get(surface);
  shell_surface->role_.toplevel_shell_surface = new ToplevelShellSurface(shell_surface);
  return surface;
}

ToplevelShellSurface *ToplevelShellSurface::Get(const Surface *surface) {
  ShellSurface *shell_surface = ShellSurface::Get(surface);

  if (nullptr == shell_surface || shell_surface->parent_) return nullptr;

  return shell_surface->role_.toplevel_shell_surface;
}

void ToplevelShellSurface::SetTitle(const char *title) const {
  p_->xdg_toplevel_.SetTitle(title);
}

void ToplevelShellSurface::SetAppId(const char *id) const {
  p_->xdg_toplevel_.SetAppId(id);
}

void ToplevelShellSurface::Move(const wayland::Seat &seat, uint32_t serial) const {
  p_->xdg_toplevel_.Move(seat, serial);
}

void ToplevelShellSurface::Resize(const wayland::Seat &seat, uint32_t serial, uint32_t edges) const {
  p_->xdg_toplevel_.Resize(seat, serial, edges);
}

void ToplevelShellSurface::SetMaximized() const {
  p_->xdg_toplevel_.SetMaximized();
}

void ToplevelShellSurface::UnsetMaximized() const {
  p_->xdg_toplevel_.UnsetMaximized();
}

void ToplevelShellSurface::SetFullscreen(const Output &output) const {
  p_->xdg_toplevel_.SetFullscreen(output.GetOutput());
}

void ToplevelShellSurface::UnsetFullscreen(const Output &output) const {
  p_->xdg_toplevel_.UnsetFullscreen(output.GetOutput());
}

void ToplevelShellSurface::SetMinimized() const {
  p_->xdg_toplevel_.SetMinimized();
}

DelegateRef<void(int, int, int)> ToplevelShellSurface::configure() {
  return p_->xdg_toplevel_.configure();
}

ToplevelShellSurface::ToplevelShellSurface(ShellSurface *shell_surface) {
  p_.reset(new Private);

  p_->shell_surface_ = shell_surface;
  p_->xdg_toplevel_.Setup(p_->shell_surface_->xdg_surface_);
}

DelegateRef<void()> ToplevelShellSurface::close() {
  return p_->xdg_toplevel_.close();
}

ToplevelShellSurface::~ToplevelShellSurface() {
  p_->xdg_toplevel_.Destroy();

  DBG_ASSERT(p_->shell_surface_->role_.toplevel_shell_surface == this);
  DBG_ASSERT(nullptr == p_->shell_surface_->parent_);
  p_->shell_surface_->role_.toplevel_shell_surface = nullptr;
}

}
