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

#ifndef SKLAND_GUI_ABSTRACT_SLIDER_HPP_
#define SKLAND_GUI_ABSTRACT_SLIDER_HPP_

#include "abstract-widget.hpp"
#include "../core/types.hpp"

#include <algorithm>

namespace skland {

template<typename T>
class AbstractSlider : public AbstractWidget {

  AbstractSlider(const AbstractSlider &) = delete;
  AbstractSlider &operator=(const AbstractSlider &) = delete;

 public:

  AbstractSlider(Orientation orientation = kHorizontal);

  virtual ~AbstractSlider();

  virtual Size GetPreferredSize() const override;

  void SetValue(const T &value);

  void SetRange(const T &value1, const T &value2);

  void SetMinimum(const T &minimum);

  void SetMaximum(const T &maximum);

  void SetOrientation(Orientation orientation);

  Orientation orientation() const {
    return orientation_;
  }

  const T &value() const {
    return value_;
  }

  const T &minimum() const {
    return minimum_;
  }

  const T &maximum() const {
    return maximum_;
  }

  const T &step() const {
    return step_;
  }

  SignalRef<const T &> changed() {
    return changed_;
  }

 protected:

  void EmitSignal() {
    changed_.Emit(value_);
  }

 private:

  Orientation orientation_;
  T value_;
  T minimum_;
  T maximum_;
  T step_;

  Signal<const T &> changed_;

};

template<typename T>
AbstractSlider<T>::AbstractSlider(Orientation orientation)
    : AbstractWidget(),
      orientation_(orientation),
      value_(T(0)),
      minimum_(T(0)),
      maximum_(T(100)),
      step_(T(1)) {

}

template<typename T>
AbstractSlider<T>::~AbstractSlider() {

}

template<typename T>
Size AbstractSlider<T>::GetPreferredSize() const {
  if (orientation_ == kHorizontal) {
    return Size(100, 20);
  } else {
    return Size(20, 100);
  }
}

template<typename T>
void AbstractSlider<T>::SetValue(const T &value) {
  if (value_ == value) return;
  if (value < minimum_ || value > maximum_) return;

  value_ = value;
  Update();
  changed_.Emit(value_);
}

template<typename T>
void AbstractSlider<T>::SetRange(const T &value1, const T &value2) {
  T minimum = std::min(value1, value2);
  T maximum = std::max(value1, value2);

  if (minimum == maximum) return;
  if (minimum_ == minimum && maximum_ == maximum) return;

  minimum_ = minimum;
  maximum_ = maximum;

  if (value_ < minimum_) value_ = minimum_;
  if (value_ > maximum_) value_ = maximum_;

  Update();
}

template<typename T>
void AbstractSlider<T>::SetMinimum(const T &minimum) {
  if (minimum_ == minimum) return;
  if ((minimum + step_) > maximum_) return;

  minimum_ = minimum;
  if (value_ < minimum_) value_ = minimum_;

  Update();
}

template<typename T>
void AbstractSlider<T>::SetMaximum(const T &maximum) {
  if (maximum_ == maximum) return;
  if ((maximum - step_) < maximum_) return;

  maximum_ = maximum;
  if (value_ > maximum_) value_ = maximum_;

  Update();
}

template<typename T>
void AbstractSlider<T>::SetOrientation(Orientation orientation) {
  if (orientation_ == orientation) return;

  orientation_ = orientation;
  // TODO: resize in parent

  Update();
}

}

#endif // SKLAND_GUI_ABSTRACT_SLIDER_HPP_
