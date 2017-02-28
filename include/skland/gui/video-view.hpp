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

#ifndef SKLAND_VIDEO_VIEW_HPP
#define SKLAND_VIDEO_VIEW_HPP

#include "abstract-view.hpp"

namespace skland {

class Surface;

/**
 * @ingroup gui
 * @brief A view with own surface to display video stream
 *
 * @note This class is currently used for test only
 */
class VideoView final : public AbstractView {

 public:

  VideoView();

  VideoView(int width, int height);

  virtual ~VideoView();

 protected:

  virtual void OnUpdate(AbstractView *view);

  virtual Surface *OnGetSurface(const AbstractView *view) const;

  virtual void OnSizeChanged(int width, int height);

  virtual void OnMouseEnter(MouseEvent *event);

  virtual void OnMouseLeave(MouseEvent *event);

  virtual void OnMouseMove(MouseEvent *event);

  virtual void OnMouseButton(MouseEvent *event);

  virtual void OnKeyboardKey(KeyEvent *event);

  virtual void OnDraw(const Context *context);

 private:

  Surface *surface_;

};

}

#endif //SKLAND_VIDEO_VIEW_HPP
