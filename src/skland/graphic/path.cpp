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

bool Path::IsInverseFillType() const {
  return sk_path_->isInverseFillType();
}

void Path::ToggleInverseFillType() {
  sk_path_->toggleInverseFillType();
}

Path::Convexity Path::GetConvexity() const {
  return (Convexity) sk_path_->getConvexity();
}

Path::Convexity Path::GetConvexityOrUnknown() const {
  return (Convexity) sk_path_->getConvexityOrUnknown();
}

void Path::SetConvexity(Convexity convexity) {
  sk_path_->setConvexity((SkPath::Convexity) convexity);
}

bool Path::IsConvex() const {
  return sk_path_->isConvex();
}

void Path::Reset() {
  sk_path_->reset();
}

void Path::Rewind() {
  sk_path_->rewind();
}

bool Path::IsEmpty() const {
  return sk_path_->isEmpty();
}

int Path::CountPoints() const {
  return sk_path_->countPoints();
}

Point2F Path::GetPoint(int index) const {
  SkPoint p = sk_path_->getPoint(index);
  return Point2F(p.fX, p.fY);
}

int Path::GetPoints(Point2F points[], int max) const {
  return sk_path_->getPoints(reinterpret_cast<SkPoint *>(points), max);
}

int Path::CountVerbs() const {
  return sk_path_->countVerbs();
}

void Path::Swap(Path &other) {
  sk_path_->swap(*other.sk_path_);
}

const RectF &Path::GetBounds() const {
  return reinterpret_cast<const RectF &>(sk_path_->getBounds());
}

void Path::UpdateBoundsCache() const {
  sk_path_->updateBoundsCache();
}

void Path::MoveTo(float x, float y) {
  sk_path_->moveTo(x, y);
}

void Path::MoveTo(const Point2F &p) {
  sk_path_->moveTo(reinterpret_cast<const SkPoint &>(p));
}

void Path::RelativeMoveTo(float dx, float dy) {
  sk_path_->rMoveTo(dx, dy);
}

void Path::LineTo(float x, float y) {
  sk_path_->lineTo(x, y);
}

void Path::LineTo(const Point2F &p) {
  sk_path_->lineTo(reinterpret_cast<const SkPoint &>(p));
}

void Path::RelativeLineTo(float dx, float dy) {
  sk_path_->rLineTo(dx, dy);
}

void Path::QuadTo(float x1, float y1, float x2, float y2) {
  sk_path_->quadTo(x1, y1, x2, y2);
}

void Path::QuadTo(const Point2F &p1, const Point2F &p2) {
  sk_path_->quadTo(reinterpret_cast<const SkPoint &>(p1), reinterpret_cast<const SkPoint &>(p2));
}

void Path::RelativeQuadTo(float dx1, float dy1, float dx2, float dy2) {
  sk_path_->rQuadTo(dx1, dy1, dx2, dy2);
}

void Path::Close() {
  sk_path_->close();
}

bool operator==(const Path &path1, const Path &path2) {
  return (*path1.sk_path_) == (*path2.sk_path_);
}

bool operator!=(const Path &path1, const Path &path2) {
  return (*path1.sk_path_) != (*path2.sk_path_);
}

}
