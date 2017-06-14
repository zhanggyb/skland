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

#ifndef SKLAND_AV_FORMAT_HPP_
#define SKLAND_AV_FORMAT_HPP_

#include "util.hpp"

extern "C" {
#include <libavformat/avformat.h>
}

#include <memory>

namespace skland {
namespace av {

// Forward declarations
class Dictionary;
class Codec;

class FormatContext;

/**
 * @ingroup av
 * @brief C++ warpper for avformat
 */
class Format {

 public:

  static unsigned int Version();

  static const char *Configuration();

  static const char *License();

  static void RegisterAll();

 private:

  static bool kAllRegistered;

};

/**
 * @ingroup av
 * @brief Input format class
 */
class InputFormat {

  friend class FormatContext;

  InputFormat() = delete;
  InputFormat(const InputFormat &) = delete;
  InputFormat &operator=(const InputFormat &) = delete;

 public:

  static std::unique_ptr<InputFormat> Find(const char *short_name);

  static std::unique_ptr<InputFormat> Get(const FormatContext &fmt_context);

  ~InputFormat() {}

  std::unique_ptr<InputFormat> next() const {
    // TODO: use iterator mode
    std::unique_ptr<InputFormat> ptr;
    if (native_) ptr.reset(new InputFormat(av_iformat_next(native_)));
    return std::move(ptr);
  }

  const char *name() const {
    return native_->name;
  }

  const char *long_name() const {
    return native_->long_name;
  }

  const char *extensions() const {
    return native_->extensions;
  }

 private:

  explicit InputFormat(AVInputFormat *input_format)
      : native_(input_format) {}

  AVInputFormat *native_;

};

// ----------------------

/**
 * @ingroup av
 * @brief AVStream wrapper
 */
class Stream {

  friend class CodecParameters;

  Stream(const Stream &) = delete;
  Stream &operator=(const Stream &) = delete;

 public:

  static std::unique_ptr<Stream> Get(const FormatContext &fmt_context, int index = 0);

  ~Stream();

  int index() const { return native_->index; }

  int id() const { return native_->id; }

  int64_t start_time() const { return native_->start_time; }

  int64_t duration() const { return native_->duration; }

  int64_t nb_frames() const { return native_->nb_frames; }

  AVCodecParameters *codecpar() const { return native_->codecpar; }

 private:

  explicit Stream(AVStream *stream);

  AVStream *native_;

};

// ----------------------

/**
 * @ingroup av
 * @brief Main class used for both muxing and demuxing
 *
 * This class encapsulate an AVFormatContext object for both muxing and
 * demuxing, which exports all information about the file being read or written.
 *
 * A FormatContext object contains:
 *   - The input or output format.
 *   - An array of AVStreams.
 *   - An I/O context.
 */
class FormatContext {

  friend class InputFormat;
  friend class Stream;

  FormatContext() = delete;
  FormatContext(const FormatContext &) = delete;
  FormatContext &operator=(const FormatContext &) = delete;

 public:

  /**
   * @brief Open an input stream and read the header
   * @param[in] url URL of the stream to open
   * @param[in] fmt If non-NULL, this parameter forces a specific input format.
   *            Otherwise the format is autodetected.
   * @param[in] dict A dictionary filled with options.
   *             On return this parameter will be reset and replaced with a dict
   *             options that were not found. May be NULL
   * @return A unique ptr to a FormatContext object
   *
   * This method will throw a runtime_error if fails.
   */
  static std::unique_ptr<FormatContext> Open(const char *url, const InputFormat *fmt, Dictionary *dict);

  /**
   * @brief Destructor
   */
  ~FormatContext();

  /**
   * @brief Close the opened input
   */
  void Close();

  /**
   * @brief Read packets of a media file to get stream information
   * @param[in/out] options
   */
  void FindStreamInfo(Dictionary *options = nullptr);

  /**
   * @brief Find the best stream in the file
   * @param media_type
   * @param wanted_stream_nb
   * @param related_stream
   * @param decoder_ret
   * @param flags
   * @return
   *
   * The best stream is determined according to various heuristics as the most likely
   * to be what the user expects. If the decoder parameter is non-NULL, this method
   * will find the default decoder for the stream's codec; streams for which no decoder
   * can be found are ignored.
   */
  int FindBestStream(MediaType media_type,
                     int wanted_stream_nb = -1,
                     int related_stream = -1,
                     Codec *decoder_ret = nullptr,
                     int flags = 0);

  /**
   * @brief Print detailed information about the input or output format
   * @param index Index of the stream to dump information about
   * @param url The URL to print, such as source or destination file
   * @param is_output Whether the specified context is an input or output
   */
  void DumpFormat(int index, const char *url, bool is_output);

  unsigned int nb_streams() const {
    return native_->nb_streams;
  }

  AVStream *streams(int n) const {
    return native_->streams[n];
  }

  int64_t start_time() const {
    return native_->start_time;
  }

  int64_t duration() const {
    return native_->duration;
  }

  unsigned int packet_size() const {
    return native_->packet_size;
  }

  int64_t bit_rate() const {
    return native_->bit_rate;
  }

 private:

  explicit FormatContext(AVFormatContext *fmt_cxt);

  AVFormatContext *native_;

  /**
   * @brief Is input opened
   */
  bool opened_;

};

}  // namespace av
}  // namespace skland

#endif  // SKLAND_AV_FORMAT_HPP_
