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

#ifndef SKLAND_GUI_ANCHOR_HPP_
#define SKLAND_GUI_ANCHOR_HPP_

#include "skland/core/types.hpp"
#include "skland/core/defines.hpp"

#include <utility>
#include <memory>

namespace skland {
namespace gui {

class AbstractView;
class AnchorGroup;

/**
 * @ingroup gui
 * @brief Used to align views in layout
 */
class Anchor {

  friend class AnchorGroup;

 public:

  SKLAND_DECLARE_NONCOPYABLE_AND_NONMOVALE(Anchor);
  Anchor() = delete;

  ~Anchor();

  Anchor *previous() const { return previous_; }

  Anchor *next() const { return next_; }

  AnchorGroup *group() const { return group_; }

  Anchor *contrary() const { return contrary_; }

  int distance() const { return *distance_; }

  void set_distance(int distance) { *distance_ = distance; }

  static std::pair<Anchor *, Anchor *> MakePair(int distance,
                                                AbstractView *view1,
                                                AbstractView *view2);

 private:

  Anchor(AbstractView *view);

  Anchor *previous_;
  Anchor *next_;
  AnchorGroup *group_;

  Anchor *contrary_;

  std::shared_ptr<int> distance_;

};

} // namespace gui
} // namespace skland

#endif // SKLAND_GUI_ANCHOR_HPP_
