//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include "skland/av/dict.hpp"

using namespace skland::av;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

TEST_F(Test, set_1) {
  Dictionary d; // Create an empty dictionary
  d.Set("foo", "bar");
  ASSERT_TRUE(d.GetCount() == 1);
}

TEST_F(Test, set_2) {
  Dictionary d;
  d.Set("foo", "bar");

  Dictionary::Entry entry = d.Get("foo", nullptr);

  bool ret1 = strcmp(entry.key(), "foo") == 0;
  bool ret2 = strcmp(entry.value(), "bar") == 0;

  ASSERT_TRUE(ret1 && ret2);
}

TEST_F(Test, copy_1) {
  Dictionary d1;
  Dictionary d2;

  d1.Set("foo", "bar");
  Dictionary::Copy(d1, d2);

  ASSERT_TRUE(d2.GetCount() == 1);
}
