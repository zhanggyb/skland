//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/deque.hpp>

using namespace skland;

class Item {

  template<typename T>
  friend
  class skland::Deque;

 public:

  Item(int id)
      : previous_(nullptr),
        next_(nullptr),
        deque_(nullptr),
        id_(id) {}

  ~Item() {
    if (deque_) deque_->Remove(this);
  }

  int id() const { return id_; };

  Item *previous() const { return previous_; }

  Item *next() const { return next_; }

  Deque<Item> *deque() const { return deque_; }

 private:

  Item *previous_;
  Item *next_;
  Deque<Item> *deque_;

  int id_;

};

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, push_front_1) {
  Item *item1 = new Item(1);
  Item *item2 = new Item(2);
  Item *item3 = new Item(3);

  Deque<Item> deque;
  deque.PushFront(item1);
  deque.PushFront(item2);
  deque.PushFront(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->next() == nullptr);
  ASSERT_TRUE(item1->previous() == item2);
  ASSERT_TRUE(item2->previous() == item3);
  ASSERT_TRUE(item3->previous() == nullptr);
}
