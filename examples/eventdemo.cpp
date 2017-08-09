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

#include <skland/gui/application.hpp>
#include <skland/gui/window.hpp>
#include <skland/gui/push-button.hpp>
#include <skland/gui/relative-layout.hpp>

using namespace skland;
using namespace skland::gui;

int main(int argc, char *argv[]) {
  Application app(argc, argv);

  Window win(400, 300, "Event Demo");
  win.SetAppId("Event-Demo");

  auto *layout = new RelativeLayout;
  auto *button = new PushButton("Test");

  layout->AddView(button);
  button->MoveTo(200, 200);

//  button->AddAnchorTo(layout, skland::kAlignLeft, 20);
//  button->AddAnchorTo(layout, skland::kAlignTop, 20);
  button->AddAnchorTo(layout, skland::kAlignRight, 20);
//  button->AddAnchorTo(layout, skland::kAlignBottom, 20);

  win.SetContentView(layout);
  win.Show();

  return app.Run();
}
