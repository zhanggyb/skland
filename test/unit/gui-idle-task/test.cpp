//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/internal/task-node.hpp>


using namespace skland::gui;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, SetNext1) {
  TaskNode* task1 = new TaskNode;
  TaskNode* task2 = new TaskNode;
  TaskNode* task3 = new TaskNode;

  task3->AddNext(task2);
  task1->AddNext(task2);
  task2->AddNext(task3);

  bool result = (task1->next() == task2) && (task2->next() == task3) &&
      (task2->previous() == task1) && (task3->previous() == task2);

  delete task1;
  delete task2;
  delete task3;

  ASSERT_TRUE(result);
}

/*
 *
 */
TEST_F(Test, Unlink1) {
  TaskNode* task1 = new TaskNode;
  TaskNode* task2 = new TaskNode;
  TaskNode* task3 = new TaskNode;

  task1->AddNext(task2);
  task2->AddNext(task3);

  task2->Unlink();
  bool result = (task2->previous() == nullptr) && (task2->next() == nullptr) &&
      (task1->next() == task3) && (task3->previous() == task1);

  delete task1;
  delete task2;
  delete task3;

  ASSERT_TRUE(result);
}


/*
 *
 */
TEST_F(Test, SetNext2) {
  TaskNode* task1 = new TaskNode;
  TaskNode* task2 = new TaskNode;
  TaskNode* task3 = new TaskNode;
  TaskNode* task4 = new TaskNode;

  task1->AddNext(task2);
  task2->AddNext(task3);
  task3->AddNext(task4);

  task3->AddNext(task2);

  bool result = (task1->next() == task3) && (task3->next() == task2) &&
      (task3->previous() == task1) && (task2->previous() == task3) &&
      (task2->next() == task4) && (task4->previous() == task2);

  delete task1;
  delete task2;
  delete task3;
  delete task4;

  ASSERT_TRUE(result);
}

/*
 *
 */
TEST_F(Test, SetPrevious1) {
  TaskNode* task1 = new TaskNode;
  TaskNode* task2 = new TaskNode;
  TaskNode* task3 = new TaskNode;

  task3->AddPrevious(task2);
  task1->AddPrevious(task2);
  task2->AddPrevious(task3);

  bool result = (task2->next() == task1) && (task3->next() == task2) &&
      (task1->previous() == task2) && (task2->previous() == task3);

  delete task1;
  delete task2;
  delete task3;

  ASSERT_TRUE(result);
}

/*
 *
 */
TEST_F(Test, SetPrevious2) {
  TaskNode* task1 = new TaskNode;
  TaskNode* task2 = new TaskNode;
  TaskNode* task3 = new TaskNode;
  TaskNode* task4 = new TaskNode;

  task1->AddPrevious(task2);
  task2->AddPrevious(task3);
  task3->AddPrevious(task4);

  task3->AddPrevious(task2);

  bool result = (task1->previous() == task3) && (task3->previous() == task2) &&
      (task3->next() == task1) && (task2->next() == task3) &&
      (task2->previous() == task4) && (task4->next() == task2);

  delete task1;
  delete task2;
  delete task3;
  delete task4;

  ASSERT_TRUE(result);
}
