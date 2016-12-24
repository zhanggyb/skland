//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/wayland/display.hpp>
#include <skland/wayland/registry.hpp>

#include <iostream>

using namespace skland::wayland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

class Case1 {

 public:
  inline Case1() {}

  inline ~Case1() {
    registry_.Destroy();
    display_.Disconnect();
  }

  void Run ();

 private:

  void OnGlobal(uint32_t id, const char *interface, uint32_t version);

  void OnGlobalRemove(uint32_t id);

  Display display_;
  Registry registry_;
};

void Case1::Run() {
  display_.Connect(NULL);

  registry_.global().Set(this, &Case1::OnGlobal);
  registry_.global_remove().Set(this, &Case1::OnGlobalRemove);
  registry_.Setup(display_);

  display_.Roundtrip();
  display_.Roundtrip();  // twice
}

void Case1::OnGlobal(uint32_t id, const char *interface, uint32_t version) {
  std::cout << "Register: id: " << id << " interface: " << interface << " version: " << version << std::endl;
}

void Case1::OnGlobalRemove(uint32_t id) {
  std::cout << "Unregister: id: " << id << std::endl;
}

TEST_F(Test, setup_1) {
  Case1 case1;
  case1.Run();

  ASSERT_TRUE(true);  // check stdout and no segment fault
}
