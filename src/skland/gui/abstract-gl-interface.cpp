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

#include "internal/abstract-gl-interface_private.hpp"

namespace skland {
namespace gui {

AbstractGLInterface::AbstractGLInterface() {

}

AbstractGLInterface::~AbstractGLInterface() {
  destroyed_.Emit();
}

void AbstractGLInterface::Setup(Surface *surface) {
  if (surface->p_->gl_interface == this) return;

  if (nullptr != surface->p_->gl_interface) {
    AbstractGLInterface *orig = surface->p_->gl_interface;
    surface->p_->gl_interface = nullptr;
    orig->OnRelease(surface);
  }

  surface->p_->gl_interface = this;

  OnSetup(surface);
}

void AbstractGLInterface::Release(Surface *surface) {
  if (surface->p_->gl_interface != this) return;

  surface->p_->gl_interface = nullptr;
  OnRelease(surface);
}

} // namespace gui
} // namespace skland
