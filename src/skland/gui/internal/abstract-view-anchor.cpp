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

#include "abstract-view-anchor.hpp"
#include "abstract-view-anchor-group.hpp"

#include "abstract-view-private.hpp"

namespace skland {

AbstractView::Anchor::Anchor(AbstractView *view)
    : contrary(nullptr),
      previous(nullptr),
      next(nullptr),
      group(nullptr) {}

AbstractView::Anchor::~Anchor() {
  if (group) group->Remove(this);

  DBG_ASSERT(nullptr == group);
  DBG_ASSERT(nullptr == previous);
  DBG_ASSERT(nullptr == next);

  if (contrary) {
    contrary->contrary = nullptr;
    delete contrary;
  }
}

std::pair<AbstractView::Anchor *, AbstractView::Anchor *> AbstractView::Anchor::MakePair(int distance,
                                                                                         AbstractView *view1,
                                                                                         AbstractView *view2) {
  Anchor *anchor1 = nullptr;
  Anchor *anchor2 = nullptr;

  DBG_ASSERT(view1->p_->parent == view2 || view1 == view2->p_->parent || view1->p_->parent == view2->p_->parent);

  anchor1 = new Anchor(view1);
  anchor2 = new Anchor(view2);
  anchor1->contrary = anchor2;
  anchor2->contrary = anchor1;
  anchor1->distance.reset(new int);
  anchor2->distance = anchor1->distance;
  *anchor1->distance = distance;

  return std::make_pair(anchor1, anchor2);
}

}
