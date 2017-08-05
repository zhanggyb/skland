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

#ifndef SKLAND_GUI_GLESV2_INTERFACE_HPP_
#define SKLAND_GUI_GLESV2_INTERFACE_HPP_

#include "abstract-gpu-interface.hpp"

namespace skland {
namespace gui {

/**
 * @ingroup gui
 * @brief OpenGL ES V2
 */
class GLESV2Interface : public AbstractGPUInterface {

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(GLESV2Interface);

  GLESV2Interface();

  virtual ~GLESV2Interface();

  virtual void SetViewportSize(int width, int height) final;

 protected:

  virtual void OnSetup() final;

 private:

  struct Private;

  void Create();

  void Destroy();

  std::unique_ptr<Private> p_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_GLESV2_INTERFACE_HPP_
