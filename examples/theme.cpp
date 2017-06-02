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
#include <skland/stock/theme.hpp>

using namespace skland;

int main(int argc, char *argv[]) {
  using skland::Window;

  Application app(argc, argv);
  Theme::Load("dark");

  Window *win = new Window(320, 240, "Dark Theme");
  win->SetAppId("Theme");

  Label *label = new Label("Hello Wayland!");
  label->SetForeground(0xFF777777);
  label->SetFont(skland::Font("Noto Sans CJK SC",
                              FontStyle(FontStyle::kWeightBold, FontStyle::kWidthNormal, FontStyle::kSlantUpright),
                              24.f));
  win->SetContentView(label);

  win->Show();

  return app.Run();
}
