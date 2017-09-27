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

set(CHROMIUM_BASE_I18N_DIR "${CHROMIUM_BASE_DIR}/i18n")

file(GLOB CHROMIUM_BASE_I18N_HEADERS "${CHROMIUM_BASE_I18N_DIR}/*.h")
file(GLOB CHROMIUM_BASE_I18N_SOURCES "${CHROMIUM_BASE_I18N_DIR}/*.cc")

# Remove sources for other platform or unit test:
file_list_remove(CHROMIUM_BASE_I18N_SOURCES GLOB "${CHROMIUM_BASE_I18N_DIR}/*_unittest.cc")
file_list_remove(CHROMIUM_BASE_I18N_SOURCES GLOB "${CHROMIUM_BASE_I18N_DIR}/*_perftest.cc")

#file_list_remove(CHROMIUM_BASE_I18N_SOURCES GLOB "${CHROMIUM_BASE_I18N_DIR}/file_util_icu.cc")
#file_list_remove(CHROMIUM_BASE_I18N_SOURCES GLOB "${CHROMIUM_BASE_I18N_DIR}/message_formatter.cc")
#file_list_remove(CHROMIUM_BASE_I18N_SOURCES GLOB "${CHROMIUM_BASE_I18N_DIR}/number_formatting.cc")
#file_list_remove(CHROMIUM_BASE_I18N_SOURCES GLOB "${CHROMIUM_BASE_I18N_DIR}/time_formatting.cc")
#file_list_remove(CHROMIUM_BASE_I18N_SOURCES GLOB "${CHROMIUM_BASE_I18N_DIR}/rtl.cc")

list(APPEND CHROMIUM_BASE_HEADERS ${CHROMIUM_BASE_I18N_HEADERS})
list(APPEND CHROMIUM_BASE_SOURCES ${CHROMIUM_BASE_I18N_SOURCES})

set(CHROMIUM_BASE_HEADERS ${CHROMIUM_BASE_HEADERS} PARENT_SCOPE)
set(CHROMIUM_BASE_SOURCES ${CHROMIUM_BASE_SOURCES} PARENT_SCOPE)