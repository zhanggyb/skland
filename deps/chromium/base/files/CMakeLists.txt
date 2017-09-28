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

set(CHROMIUM_BASE_FILES_DIR "${CHROMIUM_BASE_DIR}/files")

file(GLOB CHROMIUM_BASE_FILES_HEADERS "${CHROMIUM_BASE_FILES_DIR}/*.h")
file(GLOB CHROMIUM_BASE_FILES_SOURCES "${CHROMIUM_BASE_FILES_DIR}/*.cc")

# Remove sources for other platform or unit test:
remove_file(CHROMIUM_BASE_FILES_SOURCES GLOB "${CHROMIUM_BASE_FILES_DIR}/*_unittest.cc")
remove_file(CHROMIUM_BASE_FILES_SOURCES GLOB "${CHROMIUM_BASE_FILES_DIR}/*_fsevents.cc")
remove_file(CHROMIUM_BASE_FILES_SOURCES GLOB "${CHROMIUM_BASE_FILES_DIR}/*_kqueue.cc")
remove_file(CHROMIUM_BASE_FILES_SOURCES GLOB "${CHROMIUM_BASE_FILES_DIR}/*_mac.cc")
remove_file(CHROMIUM_BASE_FILES_SOURCES GLOB "${CHROMIUM_BASE_FILES_DIR}/*_win.cc")
remove_file(CHROMIUM_BASE_FILES_SOURCES GLOB "${CHROMIUM_BASE_FILES_DIR}/*_android.cc")
remove_file(CHROMIUM_BASE_FILES_SOURCES GLOB "${CHROMIUM_BASE_FILES_DIR}/*_fuchsia.cc")

list(APPEND CHROMIUM_BASE_HEADERS ${CHROMIUM_BASE_FILES_HEADERS})
list(APPEND CHROMIUM_BASE_SOURCES ${CHROMIUM_BASE_FILES_SOURCES})

set(CHROMIUM_BASE_HEADERS ${CHROMIUM_BASE_HEADERS} PARENT_SCOPE)
set(CHROMIUM_BASE_SOURCES ${CHROMIUM_BASE_SOURCES} PARENT_SCOPE)