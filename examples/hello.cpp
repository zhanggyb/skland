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
#include <skland/gui/label.hpp>
#include <skland/graphic/font.hpp>

int main(int argc, char *argv[]) {
  using namespace skland;
  using namespace skland::gui;
  using namespace skland::graphic;

  Application app(argc, argv);

  Window win(320, 240, "Hello");
  win.SetAppId("Hello");

  auto *label = new Label("Hello Wayland!");
  label->SetForeground(0xFF444444);
  label->SetFont(Font("Noto Sans CJK SC",
                      FontStyle(FontStyle::kWeightBold),
                      24.f));
  win.SetContentView(label);

  win.Show();

  return app.Run();
}
