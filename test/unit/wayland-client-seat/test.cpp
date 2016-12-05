//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/wayland/client/display.hpp>
#include <skland/wayland/client/registry.hpp>
#include <skland/wayland/client/seat.hpp>

#include <iostream>

using namespace skland::wayland::client;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

class Case1 {

 public:
  inline Case1() {}

  inline ~Case1() {
    seat_.Destroy();
    registry_.Destroy();
    display_.Disconnect();
  }

  void Run();

 private:

  void OnGlobal(uint32_t id, const char *interface, uint32_t version);
  void OnGlobalRemove(uint32_t id);

  void OnCapabilities(uint32_t capabilities);
  void OnName(const char *name);

  Display display_;
  Registry registry_;
  Seat seat_;
};

void Case1::Run() {
  display_.Connect(NULL);

  registry_.global().Set(this, &Case1::OnGlobal);
  registry_.global_remove().Set(this, &Case1::OnGlobalRemove);
  registry_.Setup(display_);

  seat_.capabilities().Set(this, &Case1::OnCapabilities);
  seat_.name().Set(this, &Case1::OnName);

  display_.Roundtrip();
  display_.Roundtrip();  // twice
}

void Case1::OnGlobal(uint32_t id, const char *interface, uint32_t version) {
  if (strcmp(interface, wl_seat_interface.name) == 0) {
    seat_.Setup(registry_, id, version);
  }
}

void Case1::OnGlobalRemove(uint32_t id) {
}

void Case1::OnCapabilities(uint32_t capabilities) {
  fprintf(stdout, "OnCapabilities: %d\n", capabilities);
}

void Case1::OnName(const char *name) {
  fprintf(stdout, "OnName: %s\n", name);
}

TEST_F(Test, setup_1) {
  fprintf(stdout, "Check stdout and make sure no segment fault\n");

  Case1 case1;
  case1.Run();

  ASSERT_TRUE(true);  // check stdout and no segment fault
}
