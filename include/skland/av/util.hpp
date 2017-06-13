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

#ifndef SKLAND_AV_UTIL_HPP_
#define SKLAND_AV_UTIL_HPP_

extern "C" {
#include <libavutil/avutil.h>
};

namespace skland {
namespace av {

enum MediaType {

  kMediaTypeUnknown = AVMEDIA_TYPE_UNKNOWN,
  kMediaTypeVideo = AVMEDIA_TYPE_VIDEO,
  kMediaTypeAudio = AVMEDIA_TYPE_AUDIO,
  kMediaTypeData = AVMEDIA_TYPE_DATA,
  kMediaTypeSubtitle = AVMEDIA_TYPE_SUBTITLE,
  kMediaTypeAttachment = AVMEDIA_TYPE_ATTACHMENT,
  kMediaTypeNB = AVMEDIA_TYPE_NB

};

}
}

#endif // SKLAND_AV_UTIL_HPP_
