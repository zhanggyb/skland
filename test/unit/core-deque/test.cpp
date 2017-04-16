//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/core/deque.hpp>

using namespace skland;

class Item : public Deque::Element {

 public:

  Item(int id)
      : id_(id) {}

  virtual ~Item() {}

  int id() const { return id_; };

  Item* _previous() const {
    return static_cast<Item*>(Deque::Element::previous());
  }

  Item* _next() const {
    return static_cast<Item*>(Deque::Element::next());
  }

 private:

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

  Deque deque;
  deque.PushFront(item1);
  deque.PushFront(item2);
  deque.PushFront(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->_next() == nullptr);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == nullptr);
}

TEST_F(Test, push_back_1) {
  Item *item1 = new Item(1);
  Item *item2 = new Item(2);
  Item *item3 = new Item(3);

  Deque deque;
  deque.PushBack(item1);
  deque.PushBack(item2);
  deque.PushBack(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->_previous() == nullptr);
  ASSERT_TRUE(item1->_next() == item2);
  ASSERT_TRUE(item2->_next() == item3);
  ASSERT_TRUE(item3->_next() == nullptr);
}

TEST_F(Test, insert_1) {
  Item *item1 = new Item(1);
  Item *item2 = new Item(2);
  Item *item3 = new Item(3);

  Deque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  ASSERT_TRUE(deque.count() == 3);
  ASSERT_TRUE(item1->_next() == nullptr);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == nullptr);
}

TEST_F(Test, insert_2) {
  Item *item1 = new Item(1);
  Item *item2 = new Item(2);
  Item *item3 = new Item(3);

  Deque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  Item *item4 = new Item(4);
  deque.Insert(item4);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == item4);
  ASSERT_TRUE(item4->_previous() == nullptr);
}

TEST_F(Test, insert_3) {
  Item *item1 = new Item(1);
  Item *item2 = new Item(2);
  Item *item3 = new Item(3);

  Deque deque;
  deque.Insert(item1);
  deque.Insert(item2);
  deque.Insert(item3);

  Item *item4 = new Item(4);
  deque.Insert(item4, -1);

  ASSERT_TRUE(deque.count() == 4);
  ASSERT_TRUE(item1->_previous() == item2);
  ASSERT_TRUE(item2->_previous() == item3);
  ASSERT_TRUE(item3->_previous() == nullptr);
  ASSERT_TRUE(item1->_next() == item4);
  ASSERT_TRUE(item4->_next() == nullptr);
}
