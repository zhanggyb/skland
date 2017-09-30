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

set(OIIO_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/oiio")
set(OIIO_BINARY_DIR "${PROJECT_BINARY_DIR}/deps/oiio")

file(GLOB OIIO_HEADERS "${OIIO_SOURCE_DIR}/src/include/OpenImageIO/*.h")
file(GLOB OIIO_SOURCES "${OIIO_SOURCE_DIR}/src/libOpenImageIO/*.cpp")

remove_file(OIIO_SOURCES GLOB "${OIIO_SOURCE_DIR}/src/libOpenImageIO/*_test.cpp")

#file(GLOB OIIO_LIBUTIL_HEADERS "${OIIO_SOURCE_DIR}/src/include/OpenImageIO/*.h")
file(GLOB OIIO_LIBUTIL_SOURCES "${OIIO_SOURCE_DIR}/src/libutil/*.cpp")

remove_file(OIIO_LIBUTIL_SOURCES GLOB "${OIIO_SOURCE_DIR}/src/libutil/*_test.cpp")

configure_file(
        "${OIIO_SOURCE_DIR}/src/libOpenImageIO/imageio_pvt.h.in"
        "${OIIO_BINARY_DIR}/src/libOpenImageIO/imageio_pvt.h"
        @ONLY
)

add_library(
        OpenImageIO
        ${OIIO_HEADERS}
        ${OIIO_SOURCES}
        ${OIIO_LIBUTIL_SOURCES}
        "${OIIO_BINARY_DIR}/src/libOpenImageIO/imageio_pvt.h"
)

target_include_directories(
        OpenImageIO
        PRIVATE "${OIIO_SOURCE_DIR}/src/include"
        PRIVATE "${OIIO_BINARY_DIR}/src/libOpenImageIO/"
)

target_link_libraries(
        OpenImageIO
        PRIVATE boost_filesystem
)