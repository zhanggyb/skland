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

#include "test.hpp"

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>
#include <skland/gui/relative-layout.hpp>
#include <skland/gui/push-button.hpp>
#include <skland/gui/anchor-group.hpp>
#include <skland/gui/anchor.hpp>

using skland::Application;
using skland::Window;
using skland::RelativeLayout;
using skland::PushButton;
using skland::Anchor;
using skland::AnchorGroup;

Test::Test()
    : testing::Test() {
}

Test::~Test() {

}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, regular_1) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  skland::Window win(400, 300, "Test Window");

  RelativeLayout* layout = new RelativeLayout;
  PushButton * button = new PushButton("Test");

  layout->AddView(button);
  button->MoveTo(100, 100);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}

/**
 * @brief Show a relative layout in a simple window
 *
 * Expected result: display and resize a default window
 */
TEST_F(Test, regular_2) {
  int argc = 1;
  char argv1[] = "show";  // to avoid compile warning
  char *argv[] = {argv1};

  Application app(argc, argv);

  skland::Window win(400, 300, "Test Window");

  RelativeLayout* layout = new RelativeLayout;
  PushButton * button = new PushButton("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

  button->AddAnchorTo(layout, skland::kAlignLeft, 20);
  button->AddAnchorTo(layout, skland::kAlignTop, 20);
  button->AddAnchorTo(layout, skland::kAlignRight, 20);
  button->AddAnchorTo(layout, skland::kAlignBottom, 20);

  win.SetContentView(layout);
  win.Show();

  int result = app.Run();

  ASSERT_TRUE(result == 0);
}
