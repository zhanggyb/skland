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

#ifndef SKLAND_GUI_ABSTRACT_GL_INTERFACE_HPP_
#define SKLAND_GUI_ABSTRACT_GL_INTERFACE_HPP_

#include "skland/core/defines.hpp"
#include "skland/core/sigcxx.hpp"

#include <memory>

namespace skland {
namespace gui {

class Surface;

/**
 * @ingroup gui
 * @brief The base class for graphic library to render on a 3D surface
 */
class AbstractGLInterface {

  friend class Surface;

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(AbstractGLInterface);

  AbstractGLInterface();

  virtual ~AbstractGLInterface();

  void Setup(Surface *surface);

  void Release(Surface *surface);

  virtual void SetViewportSize(int width, int height) = 0;

  virtual void MakeCurrent() = 0;

  virtual void SwapBuffers() = 0;

  core::SignalRef<> destroyed() { return destroyed_; }

 protected:

  struct Proxy;

  virtual void OnSetup(Surface *surface) = 0;

  virtual void OnRelease(Surface *surface) = 0;

 private:

  struct Private;

  core::Signal<> destroyed_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_ABSTRACT_GRAPHICS_INTERFACE_HPP_
