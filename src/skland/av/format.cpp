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

#include <skland/av/format.hpp>
#include <skland/av/dict.hpp>

#include <skland/core/assert.hpp>
#include <stdexcept>
#include <skland/av/codec.hpp>

namespace skland {
namespace av {

bool Format::kAllRegistered = false;

void Format::RegisterAll() {
  if (kAllRegistered) return;

  av_register_all();
  kAllRegistered = true;
}

// -------------------

std::unique_ptr<InputFormat> InputFormat::Find(const char *short_name) {
  std::unique_ptr<InputFormat> ptr(new InputFormat(av_find_input_format(short_name)));
  return std::move(ptr);
}

std::unique_ptr<InputFormat> InputFormat::Get(const FormatContext &fmt_context) {
  std::unique_ptr<InputFormat> ptr(new InputFormat(fmt_context.native_->iformat));
  return std::move(ptr);
}

// ----------------------

std::unique_ptr<Stream> Stream::Get(const FormatContext &fmt_context, int index) {
  std::unique_ptr<Stream> ret(new Stream(fmt_context.native_->streams[index]));
  return std::move(ret);
}

Stream::Stream(AVStream *stream)
    : native_(stream) {}

Stream::~Stream() {

}



// -------------------

FormatContext::FormatContext(AVFormatContext *fmt_cxt)
    : native_(fmt_cxt),
      opened_(false) {
  _ASSERT(native_);
}

FormatContext::~FormatContext() {
  if (opened_) avformat_close_input(&native_);
  if (native_) avformat_free_context(native_);
}

std::unique_ptr<FormatContext> FormatContext::Open(const char *url, const InputFormat *fmt, Dictionary *dict) {
  AVFormatContext *fmt_cxt = avformat_alloc_context();

  int ret = avformat_open_input(&fmt_cxt, url,
                                fmt ? fmt->native_ : NULL,
                                dict ? &dict->native_ : NULL);
  if (0 != ret) {
    _ASSERT(nullptr == native_);
    throw std::runtime_error("Error! Cannot open input stream!");
  }

  std::unique_ptr<FormatContext> ptr(new FormatContext(fmt_cxt));
  ptr->opened_ = true;

  return std::move(ptr);
}

void FormatContext::Close() {
  if (opened_) {
    _ASSERT(native_);
    avformat_close_input(&native_);
    _ASSERT(nullptr == native_);
    opened_ = false;
  }
}

void FormatContext::FindStreamInfo(Dictionary *options) {
  int ret = avformat_find_stream_info(native_, &options->native_);
  if (ret < 0) {
    throw std::runtime_error("Error! Cannot find stream info!");
  }
}

int FormatContext::FindBestStream(MediaType media_type,
                                  int wanted_stream_nb,
                                  int related_stream,
                                  Codec *decoder_ret,
                                  int flags) {
  return av_find_best_stream(native_,
                             static_cast<AVMediaType >(media_type),
                             wanted_stream_nb,
                             related_stream,
                             &decoder_ret->native_,
                             flags);
}

void FormatContext::DumpFormat(int index, const char *url, bool is_output) {
  av_dump_format(native_, index, url, is_output);
}

}  // namespace av
}  // namespace skland
