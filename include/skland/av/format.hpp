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

extern "C" {
#include <libavformat/avformat.h>
}

namespace skland {
namespace av {

// Forward declarations
class Dictionary;

/**
 * @ingroup av
 * @brief C++ warpper for avformat
 */
class Format {

 public:

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

 public:

  /**
   * @brief Default constructor
   *
   * Construct an invalid InputFormat object
   */
  InputFormat()
      : native_(nullptr) {}

  explicit InputFormat(AVInputFormat *native)
      : native_(native) {
    if (nullptr == native)
      native_ = av_iformat_next(NULL);
  }

  ~InputFormat() {}

  InputFormat(const InputFormat &orig)
      : native_(orig.native_) {}

  InputFormat &operator=(const InputFormat &other) {
    native_ = other.native_;
    return *this;
  }

  InputFormat next() const {
    if (native_) {
      AVInputFormat *f = av_iformat_next(native_);
      if (NULL != f)
        return InputFormat(f);
    }

    return InputFormat();
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

  operator bool() const {
    return nullptr != native_;
  }

 private:

  AVInputFormat *native_;

};

/**
 * @ingroup av
 * @brief Format context
 */
class FormatContext {

 public:

  FormatContext();

  ~FormatContext();

  /**
   * @brief Open an input stream and read the header
   * @param url URL of the stream to open
   * @param fmt If non-NULL, this parameter forces a specific input format.
   *            Otherwise the format is autodetected.
   * @param dict A dictionary filled with options.
   *             On return this parameter will be reset and replaced with a dict
   *             options that were not found. May be NULL
   *
   * This method will throw a runtime_error if fails.
   */
  void Open(const char *url, const InputFormat *fmt, Dictionary *dict);

  operator bool() const {
    return nullptr != native_;
  }

 private:

  AVFormatContext *native_;

};

}  // namespace av
}  // namespace skland

#endif  // SKLAND_AV_FORMAT_HPP_
