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

add_subdirectory(skia)

set(CHROMIUM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/chromium/src")
set(CHROMIUM_BINARY_DIR "${PROJECT_BINARY_DIR}/deps/chromium")
set(CHROMIUM_GEN_DIR "${CHROMIUM_BINARY_DIR}/gen")

#set(CHROMIUM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/chromium/src" CACHE STRING "Chromium source dir")
#set(CHROMIUM_BINARY_DIR "${PROJECT_BINARY_DIR}/deps/chromium" CACHE STRING "Chromium build dir")
#set(CHROMIUM_GEN_DIR "${CHROMIUM_BINARY_DIR}/gen" CACHE STRING "Chromium gen dir")

include_directories(${CHROMIUM_SOURCE_DIR})
include_directories(${CHROMIUM_GEN_DIR})

add_subdirectory(chromium)

if (LINUX)
    add_subdirectory(wayland-protocols)
endif ()
