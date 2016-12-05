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

#include <skland/graphic/path.hpp>

#include "SkPath.h"

namespace skland {

Path::Path()
    : sk_path_(nullptr) {
  sk_path_ = new SkPath;
}

Path::~Path() {
  delete sk_path_;
}

void Path::AddRoundRect(const Rect &rect, const float radii[], Direction dir) {
  sk_path_->addRoundRect(SkRect::MakeLTRB(rect.l, rect.t, rect.r, rect.b),
                         radii, static_cast<SkPath::Direction>(dir));
}

}
