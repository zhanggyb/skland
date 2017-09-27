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

set(CHROMIUM_NET_DIR "${CHROMIUM_SOURCE_DIR}/net")

file(GLOB CHROMIUM_NET_HEADERS "${CHROMIUM_NET_DIR}/*.h")
file(GLOB CHROMIUM_NET_SOURCES "${CHROMIUM_NET_DIR}/*.cc")

add_subdirectory(base)
add_subdirectory(cert)
add_subdirectory(socket)
add_subdirectory(ssl)
add_subdirectory(websockets)

add_library(
        chromium_net
        ${CHROMIUM_NET_HEADERS}
        ${CHROMIUM_NET_SOURCES}
)

target_include_directories(
        chromium_net
        PRIVATE "${CHROMIUM_SOURCE_DIR}/build/linux/debian_jessie_amd64-sysroot/usr/include/nss"
        PRIVATE "${CHROMIUM_SOURCE_DIR}/build/linux/debian_jessie_amd64-sysroot/usr/include/nspr"
        PRIVATE "${CHROMIUM_SOURCE_DIR}/third_party/boringssl/src/include"
)