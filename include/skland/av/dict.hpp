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

#ifndef SKLAND_DICT_HPP
#define SKLAND_DICT_HPP

extern "C" {
#include <libavutil/dict.h>
};

namespace skland {
namespace av {

/**
 * @ingroup av
 * @brief C++ wrapper of AVDictionary, a simple key/value pairs
 */
class Dictionary {

  friend class FormatContext;

 public:

  enum FlagType {
    kMatchCase = AV_DICT_MATCH_CASE,
    kIgnoreSuffix = AV_DICT_IGNORE_SUFFIX,
    kDontStrdupKey = AV_DICT_DONT_STRDUP_KEY,
    kDontStrdupVal = AV_DICT_DONT_STRDUP_VAL,
    kDontOverwrite = AV_DICT_DONT_OVERWRITE,
    kAppend = AV_DICT_APPEND,
    kMultikey = AV_DICT_MULTIKEY
  };

  Dictionary()
      : native_(nullptr) {}

  ~Dictionary() {
    if (native_)
      av_dict_free(&native_);
  }

  void set(const char *key, const char *value, FlagType flag) {
    int ret = av_dict_set(&native_, key, value, flag);
    if (ret < 0) {
      // TODO: throw a runtime error.
    }
  }

  int count() const {
    return av_dict_count(native_);
  }

  operator bool() const {
    return nullptr != native_;
  }

 private:

  AVDictionary *native_;

};

}
}

#endif //SKLAND_DICT_HPP
