//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/gui/output.hpp>

#include <skland/wayland/display.hpp>
#include <skland/wayland/registry.hpp>

using namespace skland;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

class Case1 {

 public:
  inline Case1()
  : output_(nullptr) {}

  inline ~Case1() {
    delete output_;
    registry_.Destroy();
    display_.Disconnect();
  }

  void Run();

 private:

  void OnGlobal(uint32_t id, const char *interface, uint32_t version);
  void OnGlobalRemove(uint32_t id);

  wayland::Display display_;
  wayland::Registry registry_;
  Output* output_;
};

void Case1::Run() {
  display_.Connect(NULL);

  registry_.global().Set(this, &Case1::OnGlobal);
  registry_.global_remove().Set(this, &Case1::OnGlobalRemove);
  registry_.Setup(display_);

  display_.Dispatch();
  display_.Roundtrip();
  display_.Roundtrip();  // twice
}

void Case1::OnGlobal(uint32_t id, const char *interface, uint32_t version) {
  if (strcmp(interface, wl_output_interface.name) == 0) {
    output_ = new Output(registry_, id, version);
  }
}

void Case1::OnGlobalRemove(uint32_t id) {
}

/*
 *
 */
TEST_F(Test, create_1) {
  fprintf(stdout, "Check stdout and make sure no segment fault\n");

  Case1 case1;
  case1.Run();

  ASSERT_TRUE(true);  // check stdout and no segment fault
}
