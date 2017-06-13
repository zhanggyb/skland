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
#include "skland/av/dict.hpp"

namespace skland {
namespace av {

Dictionary::Entry Dictionary::Get(const char *key, const Dictionary::Entry *prev, int flags) {
  return Entry(av_dict_get(native_, key, nullptr == prev ? nullptr : prev->native_, flags));
}

void Dictionary::Set(const char *key, const char *value, int flags) {
  int ret = av_dict_set(&native_, key, value, flags);
  if (ret < 0) {
    throw std::runtime_error("Error! Cannot set directory!");
  }
}

void Dictionary::Set(const char *key, int64_t value, int flags) {
  int ret = av_dict_set_int(&native_, key, value, flags);
  if (ret < 0) {
    throw std::runtime_error("Error! Cannot set directory!");
  }
}

void Dictionary::ParseString(const char *str, const char *key_val_sep, const char *pairs_sep, int flags) {
  int ret = av_dict_parse_string(&native_, str, key_val_sep, pairs_sep, flags);
  if (ret < 0) {
    throw std::runtime_error("Error! Cannot parse string!");
  }
}

void Dictionary::GetString(char **buffer, const char key_val_sep, const char pairs_sep) {
  int ret = av_dict_get_string(native_, buffer, key_val_sep, pairs_sep);
  if (ret < 0) {
    throw std::runtime_error("Error! Cannot get string from the given dictionary!");
  }
}

void Dictionary::Copy(const Dictionary &src, Dictionary &dst, int flags) {
  int ret = av_dict_copy(&dst.native_, src.native_, flags);
  if (ret < 0) {
    throw std::runtime_error("Error! Cannot copy directory!");
  }
}

} // namespace av
} // namespace skland
