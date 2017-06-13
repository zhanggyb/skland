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

#ifndef SKLAND_AV_CODEC_HPP_
#define SKLAND_AV_CODEC_HPP_

#include <bits/unique_ptr.h>
#include "format.hpp"
extern "C" {
#include <libavcodec/avcodec.h>
};

namespace skland {
namespace av {

/**
 * @ingroup av
 * @brief Structure stores compressed data
 *
 * This class encapsulate an AVPacket object.
 */
class Packet {

 public:

  /**
   * @brief Default constructor
   *
   * Allocate and initialize optional fields of a packet with default values.
   *
   * @note This only allocates the packet itself, not the data buffers. Those must be allocated through other means
   * such as av_new_packet.
   */
  Packet();

  /**
   * @brief Destructor
   *
   * The destructor will call av_packet_free(), if the packet is reference counted,
   * it will be unreferenced first.
   */
  ~Packet();

  /**
   * @brief Initialize optional fields of a packet with default values
   *
   * @note This does not touch the data and size members, which have to be initialized separately.
   */
  void Initialize();

  /**
   * @brief Allocate the payload of a packet and initialize its fields with default values
   * @param size
   */
  void Allocate(int size);

  Packet *Clone();

  /**
   * @brief Reduce packet size, correctly zeroing padding.
   * @param size
   */
  void Shrink(int size);

  /**
   * @brief Increase packet size, correctly zeroing padding.
   * @param grow_by
   */
  void Grow(int grow_by);

  /**
   * @brief Setup a new reference to the data described by a given packet
   * @param src
   *
   * If src is reference-counted, setup this as a new reference to the buffer in src.
   * Otherwise allocate a new buffer in this and copy the data from src into it.
   * All the other fields are copied from src.
   */
  void Ref(const Packet *src);

  /**
   * @brief Wipe the packet
   *
   * Unreference the buffer referenced by the packet and reset the remaining packet fields to their default values.
   */
  void Unref();

  operator bool() const {
    return nullptr != native_;
  }

  /**
   * @brief Copy packet, including contents
   * @param src
   * @param dst
   */
  static void Copy(const Packet *src, Packet *dst);

 private:

  AVPacket *native_;

};

/**
 * @ingroup av
 * @brief Coder/Decoder
 */
class Codec {

  friend class FormatContext;
  friend class CodecContext;

  Codec(const Codec &) = delete;
  Codec &operator=(const Codec &) = delete;

 public:

  static std::unique_ptr<Codec> FindDecoder(enum AVCodecID id);

  ~Codec() {}

 private:

  explicit Codec(AVCodec *codec)
      : native_(codec) {}

  AVCodec *native_;

};

/**
 * @ingroup av
 * @brief AVCodecParameters wrapper
 */
class CodecParameters {

 public:

  static CodecParameters Get(const Stream &stream) {
    return CodecParameters(stream.native_->codecpar);
  }

  CodecParameters(const CodecParameters &orig)
      : native_(orig.native_) {}

  CodecParameters &operator=(const CodecParameters &other) {
    native_ = other.native_;
    return *this;
  }

  ~CodecParameters() {}

 private:

  explicit CodecParameters(AVCodecParameters *parameters)
      : native_(parameters) {}

  AVCodecParameters *native_;

};

/**
 * @ingroup av
 * @brief Codec context
 *
 * This class encapsulate an AVCodecContext object.
 */
class CodecContext {

  CodecContext() = delete;
  CodecContext(const CodecContext &) = delete;
  CodecContext &operator=(const CodecContext &) = delete;

 public:

  static std::unique_ptr<CodecContext> Allocate(const Codec &codec);

  static std::unique_ptr<CodecContext> Allocate(const AVCodec *codec);

  ~CodecContext() {
    if (native_) avcodec_free_context(&native_);
  }

 private:

  explicit CodecContext(const AVCodec *codec)
      : native_(nullptr) {
    native_ = avcodec_alloc_context3(codec);
  }

  AVCodecContext *native_;

};

}
}

#endif // SKLAND_AV_CODEC_HPP_
