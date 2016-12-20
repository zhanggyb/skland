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

#ifndef SKLAND_GRAPHIC_PATH_HPP_
#define SKLAND_GRAPHIC_PATH_HPP_

#include "../core/rect.hpp"

class SkPath;

namespace skland {

class Canvas;

class Path {

  friend bool operator==(const Path &, const Path &);
  friend bool operator!=(const Path &, const Path &);

 public:

  enum Direction {
    kClockwise,
    kCounterClockwise
  };

  enum FillType {
    kFillTypeWinding,
    kFillTypeEvenOdd,
    kFillTypeInverseWinding,
    kFillTypeInverseEvenOdd
  };

  enum Convexity {
    kConvexityUnknown,
    kConvexityConvex,
    kConvexityConcave
  };

  Path();

  Path(const Path &other);

  ~Path();

  Path &operator=(const Path &other);

  bool IsInterpolatable(const Path &compare) const;

  bool Interpolate(const Path &ending, float weight, Path *out) const;

  void AddRoundRect(const Rect &rect, const float radii[], Direction dir = kClockwise);

  FillType GetFillType() const;

  void SetFillType(FillType fill_type);

  bool IsInverseFillType() const;

  void ToggleInverseFillType();

  Convexity GetConvexity() const;

  Convexity GetConvexityOrUnknown() const;

  void SetConvexity(Convexity convexity);

  bool IsConvex() const;

  void Reset();

  void Rewind();

  bool IsEmpty() const;

  int CountPoints() const;

  Point2F GetPoint(int index) const;

  int GetPoints(Point2F points[], int max) const;

  int CountVerbs() const;

  void Swap(Path &other);

  const RectF &GetBounds() const;

  void UpdateBoundsCache() const;

  void MoveTo(float x, float y);

  void MoveTo(const Point2F &p);

  void RelativeMoveTo(float dx, float dy);

  void LineTo(float x, float y);

  void LineTo(const Point2F &p);

  void RelativeLineTo(float dx, float dy);

  void QuadTo(float x1, float y1, float x2, float y2);

  void QuadTo(const Point2F &p1, const Point2F &p2);

  void RelativeQuadTo(float dx1, float dy1, float dx2, float dy2);

  void Close();

  SkPath *sk_path() const {
    return sk_path_;
  }

 private:

  SkPath *sk_path_;
};

bool operator==(const Path &path1, const Path &path2);

bool operator!=(const Path &path1, const Path &path2);

}

#endif // SKLAND_GRAPHIC_PATH_HPP_
