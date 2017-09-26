# Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR "${CHROMIUM_SOURCE_DIR}/third_party/ced")

add_library(chromium_third_party_ced
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/compact_enc_det/compact_enc_det.cc
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/compact_enc_det/compact_enc_det_hint_code.cc
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/util/encodings/encodings.cc
        ${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src/util/languages/languages.cc
        )
target_include_directories(
        chromium_third_party_ced
        PRIVATE "${CHROMIUM_THIRD_PARTY_CED_SOURCE_DIR}/src"
)
target_compile_options(
        chromium_third_party_ced
        PRIVATE "-Wno-narrowing"
        PRIVATE "-Wno-sign-compare"
        PRIVATE "-Wno-unused-variable"
        PRIVATE "-Wno-unused-but-set-variable"
)