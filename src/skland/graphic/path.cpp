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

Path::Path() {
  metadata_.reset(new SkPath);
}

Path::Path(const Path &other) {
  metadata_.reset(new SkPath(*other.metadata_));
}

Path::~Path() {
}

Path &Path::operator=(const Path &other) {
  *metadata_ = *other.metadata_;
  return *this;
}

bool Path::IsInterpolatable(const Path &compare) const {
  return metadata_->isInterpolatable(*compare.metadata_);
}

bool Path::Interpolate(const Path &ending, float weight, Path *out) const {
  return metadata_->interpolate(*ending.metadata_, weight, out->metadata_.get());
}

void Path::AddRect(const Rect &rect, Direction dir) {
  metadata_->addRect(reinterpret_cast<const SkRect &>(rect), static_cast<SkPath::Direction >(dir));
}

void Path::AddRect(const Rect &rect, Direction dir, unsigned start) {
  metadata_->addRect(reinterpret_cast<const SkRect &>(rect), static_cast<SkPath::Direction >(dir), start);
}

void Path::AddRect(float left, float top, float right, float bottom, Direction dir) {
  metadata_->addRect(left, top, right, bottom, static_cast<SkPath::Direction >(dir));
}

void Path::AddRoundRect(const Rect &rect, const float radii[], Direction dir) {
  metadata_->addRoundRect(reinterpret_cast<const SkRect &>(rect),
                          radii, static_cast<SkPath::Direction>(dir));
}

Path::FillType Path::GetFillType() const {
  return (FillType) metadata_->getFillType();
}

void Path::SetFillType(FillType fill_type) {
  metadata_->setFillType((SkPath::FillType) fill_type);
}

bool Path::IsInverseFillType() const {
  return metadata_->isInverseFillType();
}

void Path::ToggleInverseFillType() {
  metadata_->toggleInverseFillType();
}

Path::Convexity Path::GetConvexity() const {
  return (Convexity) metadata_->getConvexity();
}

Path::Convexity Path::GetConvexityOrUnknown() const {
  return (Convexity) metadata_->getConvexityOrUnknown();
}

void Path::SetConvexity(Convexity convexity) {
  metadata_->setConvexity((SkPath::Convexity) convexity);
}

bool Path::IsConvex() const {
  return metadata_->isConvex();
}

void Path::Reset() {
  metadata_->reset();
}

void Path::Rewind() {
  metadata_->rewind();
}

bool Path::IsEmpty() const {
  return metadata_->isEmpty();
}

int Path::CountPoints() const {
  return metadata_->countPoints();
}

Point2F Path::GetPoint(int index) const {
  SkPoint p = metadata_->getPoint(index);
  return Point2F(p.fX, p.fY);
}

int Path::GetPoints(Point2F points[], int max) const {
  return metadata_->getPoints(reinterpret_cast<SkPoint *>(points), max);
}

int Path::CountVerbs() const {
  return metadata_->countVerbs();
}

void Path::Swap(Path &other) {
  metadata_->swap(*other.metadata_);
}

const RectF &Path::GetBounds() const {
  return reinterpret_cast<const RectF &>(metadata_->getBounds());
}

void Path::UpdateBoundsCache() const {
  metadata_->updateBoundsCache();
}

void Path::MoveTo(float x, float y) {
  metadata_->moveTo(x, y);
}

void Path::MoveTo(const Point2F &p) {
  metadata_->moveTo(reinterpret_cast<const SkPoint &>(p));
}

void Path::RelativeMoveTo(float dx, float dy) {
  metadata_->rMoveTo(dx, dy);
}

void Path::LineTo(float x, float y) {
  metadata_->lineTo(x, y);
}

void Path::LineTo(const Point2F &p) {
  metadata_->lineTo(reinterpret_cast<const SkPoint &>(p));
}

void Path::RelativeLineTo(float dx, float dy) {
  metadata_->rLineTo(dx, dy);
}

void Path::QuadTo(float x1, float y1, float x2, float y2) {
  metadata_->quadTo(x1, y1, x2, y2);
}

void Path::QuadTo(const Point2F &p1, const Point2F &p2) {
  metadata_->quadTo(reinterpret_cast<const SkPoint &>(p1), reinterpret_cast<const SkPoint &>(p2));
}

void Path::RelativeQuadTo(float dx1, float dy1, float dx2, float dy2) {
  metadata_->rQuadTo(dx1, dy1, dx2, dy2);
}

void Path::Close() {
  metadata_->close();
}

bool operator==(const Path &path1, const Path &path2) {
  return (*path1.metadata_) == (*path2.metadata_);
}

bool operator!=(const Path &path1, const Path &path2) {
  return (*path1.metadata_) != (*path2.metadata_);
}

}
