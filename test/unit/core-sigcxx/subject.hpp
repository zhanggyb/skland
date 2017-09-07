//
// Created by zhanggyb on 17-9-7.
//

#ifndef SKLAND_TEST_CORE_SIGCXX_SUBJECT_HPP_
#define SKLAND_TEST_CORE_SIGCXX_SUBJECT_HPP_

#include "skland/core/sigcxx.hpp"

class Subject : public skland::core::Trackable {

 public:

  template<typename ... ParamTypes>
  using SignalRef = typename skland::core::SignalRef<ParamTypes...>;

  template<typename ... ParamTypes>
  using Signal = typename skland::core::Signal<ParamTypes...>;

  Subject() = default;

  ~Subject() final = default;

  SignalRef<int> signal1() { return signal1_; }

  SignalRef<int, int> signal2() { return signal2_; };

  void Test(int count) { signal1_.Emit(count); }

  void Test(int count1, int count2) { signal2_.Emit(count1, count2); }

 private:

  Signal<int> signal1_;
  Signal<int, int> signal2_;

};

#endif // SKLAND_TEST_CORE_SIGCXX_SUBJECT_HPP_
