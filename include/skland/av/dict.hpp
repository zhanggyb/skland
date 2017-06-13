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

#ifndef SKLAND_AV_DICT_HPP_
#define SKLAND_AV_DICT_HPP_

extern "C" {
#include <libavutil/dict.h>
};

namespace skland {
namespace av {

/**
 * @ingroup av
 * @brief A simple key/value pairs
 *
 * This class encapsulate an AVDictionary object.
 */
class Dictionary {

  friend class FormatContext;

 public:

  /**
   * @brief A nested class represents a dictionary entry
   */
  class Entry {

    friend class Dictionary;

   public:

    explicit Entry(AVDictionaryEntry *native = nullptr)
        : native_(native) {}

    Entry(const Entry &orig)
        : native_(orig.native_) {}

    ~Entry() {}

    Entry &operator=(const Entry &other) {
      native_ = other.native_;
      return *this;
    }

    Entry &operator=(AVDictionaryEntry *native) {
      native_ = native;
      return *this;
    }

    char *key() const { return native_->key; }

    char *value() const { return native_->value; }

   private:

    AVDictionaryEntry *native_;

  };

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

  Entry Get(const char *key, const Entry *prev = nullptr, int flags = 0);

  void Set(const char *key, const char *value, int flags = 0);

  /**
   * @brief set_int
   * @param key
   * @param value
   * @param flags
   */
  void Set(const char *key, int64_t value, int flags = 0);

  void ParseString(const char *str, const char *key_val_sep, const char *pairs_sep, int flags = 0);

  void GetString(char **buffer, const char key_val_sep, const char pairs_sep);

  int GetCount() const {
    return av_dict_count(native_);
  }

  operator bool() const {
    return nullptr != native_;
  }

  /**
   * @brief Copy two dictionaries
   * @param src
   * @param dst
   * @param flags
   */
  static void Copy(const Dictionary &src, Dictionary &dst, int flags = 0);

 private:

  AVDictionary *native_;

};

}
}

#endif // SKLAND_AV_DICT_HPP_
