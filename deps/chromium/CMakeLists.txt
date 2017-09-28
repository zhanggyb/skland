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

set(CHROMIUM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/chromium/src")
set(CHROMIUM_BINARY_DIR "${PROJECT_BINARY_DIR}/deps/chromium")
set(CHROMIUM_GEN_DIR "${CHROMIUM_BINARY_DIR}/gen")

#set(CHROMIUM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/chromium/src" CACHE STRING "Chromium source dir")
#set(CHROMIUM_BINARY_DIR "${PROJECT_BINARY_DIR}/deps/chromium" CACHE STRING "Chromium build dir")
#set(CHROMIUM_GEN_DIR "${CHROMIUM_BINARY_DIR}/gen" CACHE STRING "Chromium gen dir")

include_directories(${CHROMIUM_SOURCE_DIR})
include_directories(${CHROMIUM_GEN_DIR})

#cmake_minimum_required(VERSION 3.1)

#project(Chromium)

# The project version number.
#set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
#set(VERSION_MINOR 0 CACHE STRING "Project minor version number.")
#set(VERSION_PATCH 1 CACHE STRING "Project patch version number.")
#mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

#option(CHROMIUM_SOURCE_DIR "Directory of chromium source")
#option(CHROMIUM_BINARY_DIR "Directory to build chromium")
#option(CHROMIUM_GEN_DIR "Directory for generated files")

#if (NOT CHROMIUM_SOURCE_DIR)
#    set(CHROMIUM_SOURCE_DIR ${PROJECT_SOURCE_DIR})
#endif ()

#if (NOT CHROMIUM_BINARY_DIR)
#    set(CHROMIUM_BINARY_DIR ${PROJECT_BINARY_DIR})
#endif ()

#if (NOT CHROMIUM_GEN_DIR)
#    set(CHROMIUM_GEN_DIR "${CHROMIUM_BINARY_DIR}/gen")
#endif ()

add_subdirectory(third_party)
add_subdirectory(base)
add_subdirectory(net)

add_custom_target(
        chromium
        COMMENT "Build chromium libraries"
)

add_dependencies(chromium chromium_base)
