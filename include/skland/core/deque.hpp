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

#ifndef SKLAND_CORE_DEQUE_HPP_
#define SKLAND_CORE_DEQUE_HPP_

#include "defines.hpp"

namespace skland {

class Deque {

 public:

  class Element {

    friend class Deque;

   public:

    Element();

    virtual ~Element();

    Element *previous() const { return previous_; }

    Element *next() const { return next_; }

    Deque *deque() const { return deque_; }

   private:

    Element *previous_;
    Element *next_;
    Deque *deque_;

  };

  Deque();

  ~Deque();

  void PushFront(Element *item);

  void PushBack(Element *item);

  void Insert(Element *item, int index = 0);

  Element *Remove(Element *item);

  void Clear();

  Element *operator[](int index) const;

  Element *first() const { return first_; }

  Element *last() const { return last_; }

  int count() const { return count_; }

 private:

  Element *first_;
  Element *last_;
  int count_;

};

}

#endif // SKLAND_CORE_DEQUE_HPP_
