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

# TODO: this module requires nss, ssl

set(CHROMIUM_NET_SSL_DIR "${CHROMIUM_NET_DIR}/ssl")

file(GLOB CHROMIUM_NET_SSL_HEADERS "${CHROMIUM_NET_SSL_DIR}/*.h")
file(GLOB CHROMIUM_NET_SSL_SOURCES "${CHROMIUM_NET_SSL_DIR}/*.cc")

# Remove headers for other platform or unit test:
remove_file(CHROMIUM_NET_SSL_HEADERS GLOB "${CHROMIUM_NET_SSL_DIR}/*_win.h")

# Remove sources for other platform or unit test:
remove_file(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_unittest.cc")
#file_list_remove(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_test.cc")
remove_file(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_test_util.cc")
#file_list_remove(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_perftest.cc")
remove_file(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/test_*.cc")
remove_file(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_win.cc")
#file_list_remove(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/winsock_*.cc")
remove_file(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_mac.cc")
remove_file(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_android.cc")
#file_list_remove(CHROMIUM_NET_SSL_SOURCES GLOB "${CHROMIUM_NET_SSL_DIR}/*_fuchsia.cc")


list(APPEND CHROMIUM_NET_HEADERS ${CHROMIUM_NET_SSL_HEADERS})
list(APPEND CHROMIUM_NET_SOURCES ${CHROMIUM_NET_SSL_SOURCES})

set(CHROMIUM_NET_HEADERS ${CHROMIUM_NET_HEADERS} PARENT_SCOPE)
set(CHROMIUM_NET_SOURCES ${CHROMIUM_NET_SOURCES} PARENT_SCOPE)
