//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/task.hpp>

using skland::gui::Task;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/*
 *
 */
TEST_F(Test, SetNext1) {
  Task *task1 = new Task;
  Task *task2 = new Task;
  Task *task3 = new Task;

  task3->PushBack(task2);
  task1->PushBack(task2);
  task2->PushBack(task3);

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
  Task *task1 = new Task;
  Task *task2 = new Task;
  Task *task3 = new Task;

  task1->PushBack(task2);
  task2->PushBack(task3);

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
  Task *task1 = new Task;
  Task *task2 = new Task;
  Task *task3 = new Task;
  Task *task4 = new Task;

  task1->PushBack(task2);
  task2->PushBack(task3);
  task3->PushBack(task4);

  task3->PushBack(task2);

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
  Task *task1 = new Task;
  Task *task2 = new Task;
  Task *task3 = new Task;

  task3->PushFront(task2);
  task1->PushFront(task2);
  task2->PushFront(task3);

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
  Task *task1 = new Task;
  Task *task2 = new Task;
  Task *task3 = new Task;
  Task *task4 = new Task;

  task1->PushFront(task2);
  task2->PushFront(task3);
  task3->PushFront(task4);

  task3->PushFront(task2);

  bool result = (task1->previous() == task3) && (task3->previous() == task2) &&
      (task3->next() == task1) && (task2->next() == task3) &&
      (task2->previous() == task4) && (task4->next() == task2);

  delete task1;
  delete task2;
  delete task3;
  delete task4;

  ASSERT_TRUE(result);
}

TEST_F(Test, PushBackLink1) {
  Task *task1 = new Task;
  Task *task2 = new Task;
  Task *task3 = new Task;

  Task *task4 = new Task;
  Task *task5 = new Task;

  // 1 - 5
  task1->PushBack(task5);

  // 2 - 3 - 4
  task2->PushBack(task3);
  task3->PushBack(task4);

  // 1 - 2 - 3 - 4 - 5
  task1->PushBack(task2, task4);

  bool resut = ((task1->next() == task2) && (task2->previous() == task1) &&
      (task2->next() == task3) && (task3->previous() == task2) &&
      (task3->next() == task4) && (task4->previous() == task3) &&
      (task4->next() == task5) && (task5->previous() == task4));

  delete task1;
  delete task2;
  delete task3;
  delete task4;
  delete task5;

  ASSERT_TRUE(resut);
}

TEST_F(Test, PushFrontLink1) {
  Task *task1 = new Task;
  Task *task2 = new Task;
  Task *task3 = new Task;

  Task *task4 = new Task;
  Task *task5 = new Task;

  // 1 - 5
  task1->PushBack(task5);

  // 2 - 3 - 4
  task2->PushBack(task3);
  task3->PushBack(task4);

  // 1 - 2 - 3 - 4 - 5
  task5->PushFront(task2, task4);

  bool resut = ((task1->next() == task2) && (task2->previous() == task1) &&
      (task2->next() == task3) && (task3->previous() == task2) &&
      (task3->next() == task4) && (task4->previous() == task3) &&
      (task4->next() == task5) && (task5->previous() == task4));

  delete task1;
  delete task2;
  delete task3;
  delete task4;
  delete task5;

  ASSERT_TRUE(resut);
}
