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

Path::Path(const Path &other)
    : sk_path_(nullptr) {
  sk_path_ = new SkPath(*other.sk_path_);
}

Path::~Path() {
  delete sk_path_;
}

Path &Path::operator=(const Path &other) {
  *sk_path_ = *other.sk_path_;
  return *this;
}

bool Path::IsInterpolatable(const Path &compare) const {
  return sk_path_->isInterpolatable(*compare.sk_path_);
}

bool Path::Interpolate(const Path &ending, float weight, Path *out) const {
  return sk_path_->interpolate(*ending.sk_path_, weight, out->sk_path_);
}

void Path::AddRoundRect(const Rect &rect, const float radii[], Direction dir) {
  sk_path_->addRoundRect(reinterpret_cast<const SkRect &>(rect),
                         radii, static_cast<SkPath::Direction>(dir));
}

Path::FillType Path::GetFillType() const {
  return (FillType) sk_path_->getFillType();
}

void Path::SetFillType(FillType fill_type) {
  sk_path_->setFillType((SkPath::FillType) fill_type);
}

bool operator==(const Path &path1, const Path &path2) {
  return (*path1.sk_path_) == (*path2.sk_path_);
}

bool operator!=(const Path &path1, const Path &path2) {
  return (*path1.sk_path_) != (*path2.sk_path_);
}

}
