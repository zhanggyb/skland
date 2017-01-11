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

#ifndef SKLAND_GUI_ABSTRACT_SLIDER_BAR_HPP_
#define SKLAND_GUI_ABSTRACT_SLIDER_BAR_HPP_

#include "abstract-widget.hpp"

namespace skland {

class AbstractSliderBar : public AbstractWidget {

  AbstractSliderBar(const AbstractSliderBar &) = delete;
  AbstractSliderBar &operator=(const AbstractSliderBar &) = delete;

 public:

  AbstractSliderBar();

  virtual ~AbstractSliderBar();

  virtual Size GetPreferredSize() const override;

  SignalRef<double> changed() {
    return changed_;
  }

  double minimum() const {
    return minimum_;
  }

  double value() const {
    return value_;
  }

  double maximum() const {
    return maximum_;
  }

 protected:

  void EmitSignal() {
    changed_.Emit(value_);
  }

 private:

  Signal<double> changed_;

  double minimum_;

  double value_;

  double maximum_;

};

}

#endif // SKLAND_GUI_ABSTRACT_SLIDER_BAR_HPP_
