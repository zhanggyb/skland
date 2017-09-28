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

set(CHROMIUM_BASE_DIR "${CHROMIUM_SOURCE_DIR}/base")

file(GLOB CHROMIUM_BASE_HEADERS "${CHROMIUM_BASE_DIR}/*.h")
file(GLOB CHROMIUM_BASE_SOURCES "${CHROMIUM_BASE_DIR}/*.cc")

# Remove sources for other platform or unit test:
remove_file(CHROMIUM_BASE_HEADERS GLOB "${CHROMIUM_BASE_DIR}/*_win.h")
remove_file(CHROMIUM_BASE_HEADERS GLOB "${CHROMIUM_BASE_DIR}/*_mac.h")
remove_file(CHROMIUM_BASE_HEADERS GLOB "${CHROMIUM_BASE_DIR}/*_android.h")

remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_unittest.cc")
remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_win.cc")
remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_nacl.cc")
remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_android.cc")
remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_chromeos.cc")
remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_freebsd.cc")
remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_openbsd.cc")
remove_file(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_fuchsia.cc")

add_subdirectory(allocator)
add_subdirectory(containers)
add_subdirectory(debug)
add_subdirectory(files)
add_subdirectory(i18n) # TODO: fix compile error
add_subdirectory(json)
add_subdirectory(memory)
add_subdirectory(message_loop)
add_subdirectory(metrics)
add_subdirectory(nix)
add_subdirectory(numerics)
add_subdirectory(posix)
add_subdirectory(power_monitor)
add_subdirectory(process)
add_subdirectory(profiler)
add_subdirectory(strings)
add_subdirectory(synchronization)
add_subdirectory(system_monitor)
add_subdirectory(task)
add_subdirectory(task_scheduler)
add_subdirectory(trace_event)
add_subdirectory(threading)
add_subdirectory(time)
add_subdirectory(timer)

# TODO: use a global variable for these definitions as they are also used when building icu
add_definitions(-DU_USING_ICU_NAMESPACE=0)
add_definitions(-DU_ENABLE_DYLOAD=0)
#add_definitions(-DU_COMMON_IMPLEMENTATION)
#add_definitions(-DU_I18N_IMPLEMENTATION)
add_definitions(-DU_STATIC_IMPLEMENTATION)
add_definitions(-DICU_UTIL_DATA_IMPL=ICU_UTIL_DATA_STATIC)
add_definitions(-DUCHAR_TYPE=uint16_t)

add_library(chromium_base ${CHROMIUM_BASE_HEADERS} ${CHROMIUM_BASE_SOURCES})
target_include_directories(
        chromium_base
        PRIVATE "${CHROMIUM_SOURCE_DIR}/third_party/icu/source/common/"
        PRIVATE "${CHROMIUM_SOURCE_DIR}/third_party/ced/src"
)
target_link_libraries(
        chromium_base
        PRIVATE chromium_third_party_icu
)

# Generate files before build chromium_base:

add_custom_target(
        chromium_base_generate_headers
        COMMENT "Generate header files for chromium/base in ${CHROMIUM_GEN_DIR}/base"
        VERBATIM
)

add_custom_command(
        TARGET chromium_base_generate_headers
        POST_BUILD
        COMMAND mkdir -p "${CHROMIUM_GEN_DIR}/base"
        WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
        COMMENT "Create ${CHROMIUM_GEN_DIR}/base"
        VERBATIM
)

# The chromium build system generates some header files in 'gen/base/' dir.
# The following custom commands do the same thing, though ugly.
# TODO: move these commands to configure stage.

set(DEFINITION_CONTENT "--flags ENABLE_LOCATION_SOURCE=true ENABLE_PROFILING=false CAN_UNWIND_WITH_FRAME_POINTERS=true")
set(SCRIPT "${CHROMIUM_SOURCE_DIR}/build/write_buildflag_header.py")
set(TMPFILE "${CHROMIUM_BINARY_DIR}/base/debugging_flags.rsp")
add_custom_command(
        TARGET chromium_base_generate_headers
        POST_BUILD
        COMMAND mkdir -p "${CHROMIUM_GEN_DIR}/base/debug"
        COMMAND echo "${DEFINITION_CONTENT}" > "${TMPFILE}"
        COMMAND python "${SCRIPT}" --output base/debug/debugging_flags.h --rulename //base$:debugging_flags --gen-dir "${CHROMIUM_GEN_DIR}" --definitions "${TMPFILE}"
        COMMAND rm -f "${TMPFILE}"
        WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
        COMMENT "Generate ${CHROMIUM_GEN_DIR}/base/debug/debugging_flags.h"
        VERBATIM
)

set(DEFINITION_CONTENT "--flags USE_ALLOCATOR_SHIM=true")
set(SCRIPT "${CHROMIUM_SOURCE_DIR}/build/write_buildflag_header.py")
set(TMPFILE "${CHROMIUM_BINARY_DIR}/base/features.rsp")
add_custom_command(
        TARGET chromium_base_generate_headers
        POST_BUILD
        COMMAND mkdir -p "${CHROMIUM_GEN_DIR}/base/allocator"
        COMMAND echo "${DEFINITION_CONTENT}" > "${TMPFILE}"
        COMMAND python "${SCRIPT}" --output base/allocator/features.h --rulename //base/allocator$:features --gen-dir "${CHROMIUM_GEN_DIR}" --definitions "${TMPFILE}"
        COMMAND rm -f "${TMPFILE}"
        WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
        COMMENT "Generate ${CHROMIUM_GEN_DIR}/base/allocator/features.h"
        VERBATIM
)

set(SCRIPT "${CHROMIUM_SOURCE_DIR}/build/write_build_date_header.py")
add_custom_command(
        TARGET chromium_base_generate_headers
        POST_BUILD
        COMMAND "${CHROMIUM_SOURCE_DIR}/build/write_build_date_header.py" "${CHROMIUM_GEN_DIR}/base/generated_build_date.h" default
        WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
        COMMENT "Generate ${CHROMIUM_GEN_DIR}/base/generated_build_date.h"
        VERBATIM
)

#add_custom_command(
#	TARGET chromium_base_generate_headers
#	POST_BUILD
#	COMMAND "${CHROMIUM_SOURCE_DIR}/build/write_buildflag_header.py" "${CHROMIUM_GEN_DIR}/base/debug/debugging_flags.h" default "${CHROMIUM_SOURCE_DIR}/build/util/LASTCHANGE"
#	WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
#	COMMENT "Generate ${CHROMIUM_GEN_DIR}/base/debug/debugging_flags.h by build/write_buildflag_header.py"
#	VERBATIM
#)

#add_custom_command(
#	TARGET chromium_base_generate_headers
#	POST_BUILD
#	COMMAND "${CHROMIUM_SOURCE_DIR}/build/write_buildflag_header.py" "${CHROMIUM_GEN_DIR}/base/allocator/features.h" default "${CHROMIUM_SOURCE_DIR}/build/util/LASTCHANGE"
#	WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
#	COMMENT "Generate ${CHROMIUM_GEN_DIR}/base/allocator/features.h by build/write_buildflag_header.py"
#	VERBATIM
#)

# use add_custom_target instead and add_dependencies to make the gen target depend on it
add_dependencies(
        chromium_base
        chromium_base_generate_headers
)

