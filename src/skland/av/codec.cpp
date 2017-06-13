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

#include <stdexcept>
#include "skland/av/codec.hpp"

namespace skland {
namespace av {

Packet::Packet()
    : native_(nullptr) {
  native_ = av_packet_alloc();
  if (!native_)
    fprintf(stderr, "Error! Fail to allocate packet!");
}

Packet::~Packet() {
  if (native_) av_packet_free(&native_);
}

void Packet::Initialize() {
  av_init_packet(native_);
}

void Packet::Allocate(int size) {
  int ret = av_new_packet(native_, size);
  if (ret < 0) {
    throw std::runtime_error("Error! Fail to allocate payload!");
  }
}

void Packet::Ref(const Packet *src) {
  int ret = av_packet_ref(native_, src->native_);
  if (ret < 0) {
    throw std::runtime_error("Error! Fail to setup a new reference to the given packet!");
  }
}

void Packet::Unref() {
  av_packet_unref(native_);
}

void Packet::Copy(const Packet *src, Packet *dst) {
  int ret = av_copy_packet(dst->native_, src->native_);
  if (ret < 0) {
    throw std::runtime_error("Error! Fail to copy packet!");
  }
}

// --------------------

std::unique_ptr<Codec> Codec::FindDecoder(enum AVCodecID id) {
  AVCodec *codec = avcodec_find_decoder(id);
  if (!codec) {
    throw std::runtime_error("Error! Fail to find decoder from given ID!");
  }

  std::unique_ptr<Codec> ptr(new Codec(codec));
  return std::move(ptr);
}

// -------------------

std::unique_ptr<CodecContext> CodecContext::Allocate(const Codec &codec) {
  std::unique_ptr<CodecContext> ptr(new CodecContext(codec.native_));
  return std::move(ptr);
}

std::unique_ptr<CodecContext> CodecContext::Allocate(const AVCodec *codec) {
  std::unique_ptr<CodecContext> ptr(new CodecContext(codec));
  return std::move(ptr);
}

} // namespace av
} // namespace skland
