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
#include <skland/av/format.hpp>
#include <skland/av/dict.hpp>
#include <skland/av/codec.hpp>
#include <skland/av/util.hpp>

#include <iostream>

template<typename T>
using uni_ptr = std::unique_ptr<T>;

int main(int argc, char *argv[]) {
  using namespace skland;
  using namespace skland::av;

  Application app(argc, argv);
  av::Format::RegisterAll();

  std::string url = "file:";
  if (argc > 1) {
    url += argv[1];
  } else {
    url += "in.mp3";
  }

  uni_ptr<FormatContext> context;

  try {
    context = FormatContext::Open(url.c_str(), nullptr, nullptr);
    uni_ptr<InputFormat> f = InputFormat::Get(*context);

    std::cout << f->name() << std::endl;
    std::cout << f->long_name() << std::endl;

    context->FindStreamInfo();

    int ret = context->FindBestStream(av::kMediaTypeVideo);
    std::cout << "best stream: " << ret << std::endl;

    if (ret < 0) { abort(); }

    uni_ptr<Stream> stream = Stream::Get(*context, ret);

    uni_ptr<Codec> dec = Codec::FindDecoder(stream->codecpar()->codec_id);

  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    app.Exit();
  }

  fprintf(stdout, "Press Ctrl-C to exit the main loop.\n");

  return app.Run();
}
