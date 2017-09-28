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

set(CHROMIUM_NET_CERT_DIR "${CHROMIUM_NET_DIR}/cert")

file(GLOB CHROMIUM_NET_CERT_HEADERS "${CHROMIUM_NET_CERT_DIR}/*.h")
file(GLOB CHROMIUM_NET_CERT_SOURCES "${CHROMIUM_NET_CERT_DIR}/*.cc")

# Remove headers for other platform or unit test:
remove_file(CHROMIUM_NET_CERT_HEADERS GLOB "${CHROMIUM_NET_CERT_DIR}/*_win.h")

# Remove sources for other platform or unit test:
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_unittest.cc")
#file_list_remove(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_test.cc")
#file_list_remove(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_test_util.cc")
#file_list_remove(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_perftest.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/test_*.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_win.cc")
#file_list_remove(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/winsock_*.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_mac.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_android.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_ios.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_chromeos.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/*_fuchsia.cc")

# Remove nss coude:
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/cert_verify_proc_nss.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/cert_verify_proc.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/known_roots_nss.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/nss_cert_database.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/x509_certificate_nss.cc")
remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/x509_util_nss.cc")

remove_file(CHROMIUM_NET_CERT_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/test_*.cc")

# internal sources:

file(GLOB CHROMIUM_NET_CERT_INTERNAL_HEADERS "${CHROMIUM_NET_CERT_DIR}/internal/*.h")
file(GLOB CHROMIUM_NET_CERT_INTERNAL_SOURCES "${CHROMIUM_NET_CERT_DIR}/internal/*.cc")

remove_file(CHROMIUM_NET_CERT_INTERNAL_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/internal/*_unittest.cc")
remove_file(CHROMIUM_NET_CERT_INTERNAL_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/internal/*_mac.cc")

remove_file(CHROMIUM_NET_CERT_INTERNAL_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/internal/*_nss.cc")

remove_file(CHROMIUM_NET_CERT_INTERNAL_SOURCES GLOB "${CHROMIUM_NET_CERT_DIR}/internal/test_*.cc")

list(APPEND CHROMIUM_NET_HEADERS ${CHROMIUM_NET_CERT_HEADERS} ${CHROMIUM_NET_CERT_INTERNAL_HEADERS})
list(APPEND CHROMIUM_NET_SOURCES ${CHROMIUM_NET_CERT_SOURCES} ${CHROMIUM_NET_CERT_INTERNAL_SOURCES})

set(CHROMIUM_NET_HEADERS ${CHROMIUM_NET_HEADERS} PARENT_SCOPE)
set(CHROMIUM_NET_SOURCES ${CHROMIUM_NET_SOURCES} PARENT_SCOPE)
