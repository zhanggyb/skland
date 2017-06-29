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

#ifndef SKLAND_GUI_ABSTRACT_GRAPHIC_ENGINE_HPP_
#define SKLAND_GUI_ABSTRACT_GRAPHIC_ENGINE_HPP_

namespace skland {
namespace gui {

class Surface;

/**
 * @brief The base class for graphic engine to render on a 3D surface
 */
class AbstractGraphicEngine {

 public:

  AbstractGraphicEngine();

  virtual ~AbstractGraphicEngine();

  /**
   * @brief Setup the surface on which this engine works
   * @param surface
   */
  virtual void Setup(Surface *surface) = 0;

 protected:

  struct Proxy;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_ABSTRACT_GRAPHIC_ENGINE_HPP_
