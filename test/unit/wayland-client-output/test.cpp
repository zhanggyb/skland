//
// Created by zhanggyb on 16-9-19.
//

#include "test.hpp"

#include <skland/wayland/display.hpp>
#include <skland/wayland/registry.hpp>
#include <skland/wayland/output.hpp>

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
    output_.Destroy();
    registry_.Destroy();
    display_.Disconnect();
  }

  void Run();

 private:

  void OnGlobal(uint32_t id, const char *interface, uint32_t version);
  void OnGlobalRemove(uint32_t id);

  // Output callbacks
  void OnGeometry(int32_t x,
                  int32_t y,
                  int32_t physical_width,
                  int32_t physical_height,
                  int32_t subpixel,
                  const char *make,
                  const char *model,
                  int32_t transform);

  void OnMode(uint32_t flags,
              int32_t width,
              int32_t height,
              int32_t refresh);

  void OnDone();

  void OnScale(int32_t factor);

  Display display_;
  Registry registry_;
  Output output_;
};

void Case1::Run() {
  display_.Connect(NULL);

  registry_.global().Set(this, &Case1::OnGlobal);
  registry_.global_remove().Set(this, &Case1::OnGlobalRemove);
  registry_.Setup(display_);

  output_.geometry().Set(this, &Case1::OnGeometry);
  output_.mode().Set(this, &Case1::OnMode);
  output_.done().Set(this, &Case1::OnDone);
  output_.scale().Set(this, &Case1::OnScale);

  display_.Dispatch();
  display_.Roundtrip();
  display_.Roundtrip();  // twice
}

void Case1::OnGlobal(uint32_t id, const char *interface, uint32_t version) {
  if (strcmp(interface, wl_output_interface.name) == 0) {
    output_.Setup(registry_, id, version);
  }
}

void Case1::OnGlobalRemove(uint32_t id) {
}

void Case1::OnGeometry(int32_t x,
                       int32_t y,
                       int32_t physical_width,
                       int32_t physical_height,
                       int32_t subpixel,
                       const char *make,
                       const char *model,
                       int32_t transform) {
  fprintf(stdout,
          "OnGeometry: x: %d, y: %d, physical_width: %d, physical_height: %d, subpixel: %d, make: %s, model: %s, transform: %d\n",
          x,
          y,
          physical_width,
          physical_height,
          subpixel,
          make,
          model,
          transform);
}

void Case1::OnMode(uint32_t flags, int32_t width, int32_t height, int32_t refresh) {
  fprintf(stdout, "OnMode: flags: %d, width: %d, height: %d, refresh: %d\n", flags, width, height, refresh);
}

void Case1::OnDone() {
  fprintf(stdout, "OnDone\n");
}

void Case1::OnScale(int32_t factor) {
  fprintf(stdout, "OnScale: factor: %d\n", factor);
}

// ----

class Wrapper {

  Wrapper() = delete;

 public:

  Wrapper(const Registry &registry, uint32_t id, uint32_t version) {
    output_.geometry().Set(this, &Wrapper::OnGeometry);
    output_.mode().Set(this, &Wrapper::OnMode);
    output_.done().Set(this, &Wrapper::OnDone);
    output_.scale().Set(this, &Wrapper::OnScale);
    output_.Setup(registry, id, version);
    output_.SetUserData(this);  // this line sometimes does not work, move into OnDone()
  }

  ~Wrapper() {}

 private:

  // Output callbacks
  void OnGeometry(int32_t x,
                  int32_t y,
                  int32_t physical_width,
                  int32_t physical_height,
                  int32_t subpixel,
                  const char *make,
                  const char *model,
                  int32_t transform) {
    fprintf(stdout,
            "OnGeometry: x: %d, y: %d, physical_width: %d, physical_height: %d, subpixel: %d, make: %s, model: %s, transform: %d\n",
            x,
            y,
            physical_width,
            physical_height,
            subpixel,
            make,
            model,
            transform);
  }

  void OnMode(uint32_t flags,
              int32_t width,
              int32_t height,
              int32_t refresh) {
    fprintf(stdout, "OnMode: flags: %d, width: %d, height: %d, refresh: %d\n", flags, width, height, refresh);
  }

  void OnDone() {
    fprintf(stdout, "OnDone\n");
  }

  void OnScale(int32_t factor) {
    fprintf(stdout, "OnScale: factor: %d\n", factor);
  }

  Output output_;
};

class Case2 {

 public:
  inline Case2()
      : output_(nullptr) {}

  inline ~Case2() {
    delete output_;
    registry_.Destroy();
    display_.Disconnect();
  }

  void Run();

 private:

  void OnGlobal(uint32_t id, const char *interface, uint32_t version);
  void OnGlobalRemove(uint32_t id);

  Display display_;
  Registry registry_;
  Wrapper *output_;
};

void Case2::Run() {
  display_.Connect(NULL);

  registry_.global().Set(this, &Case2::OnGlobal);
  registry_.global_remove().Set(this, &Case2::OnGlobalRemove);
  registry_.Setup(display_);

  display_.Dispatch();
  display_.Roundtrip();
  display_.Roundtrip();  // twice
}

void Case2::OnGlobal(uint32_t id, const char *interface, uint32_t version) {
  if (strcmp(interface, wl_output_interface.name) == 0) {
    if (output_ == nullptr)
      output_ = new Wrapper(registry_, id, version);
  }
}

void Case2::OnGlobalRemove(uint32_t id) {
}

// ----

TEST_F(Test, setup_1) {
  fprintf(stdout, "Check stdout and make sure no segment fault\n");

  Case1 case1;
  case1.Run();

  ASSERT_TRUE(true);  // check stdout and no segment fault
}

TEST_F(Test, setup_2) {
  fprintf(stdout, "Check stdout and make sure no segment fault\n");

  Case2 case2;
  case2.Run();

  ASSERT_TRUE(true);  // check stdout and no segment fault
}
