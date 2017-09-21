# deps/chromium/base

set(CHROMIUM_BASE_DIR "${CHROMIUM_SOURCE_DIR}/base")

file(GLOB CHROMIUM_BASE_HEADERS "${CHROMIUM_BASE_DIR}/*.h")
file(GLOB CHROMIUM_BASE_SOURCES "${CHROMIUM_BASE_DIR}/*.cc")

# Remove sources for other platform or unit test:
file_list_remove(CHROMIUM_BASE_HEADERS GLOB "${CHROMIUM_BASE_DIR}/*_win.h")
file_list_remove(CHROMIUM_BASE_HEADERS GLOB "${CHROMIUM_BASE_DIR}/*_mac.h")
file_list_remove(CHROMIUM_BASE_HEADERS GLOB "${CHROMIUM_BASE_DIR}/*_android.h")

file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_unittest.cc")
file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_win.cc")
file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_nacl.cc")
file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_android.cc")
file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_chromeos.cc")
file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_freebsd.cc")
file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_openbsd.cc")
file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_fuchsia.cc")

add_subdirectory(allocator)
add_subdirectory(containers)
add_subdirectory(debug)
add_subdirectory(files)
#add_subdirectory(i18n) # TODO: fix compile error
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

include_directories(BEFORE ${CHROMIUM_SOURCE_DIR}/third_party/icu/source/common/)

add_library(chromium_base ${CHROMIUM_BASE_HEADERS} ${CHROMIUM_BASE_SOURCES})

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
        COMMENT "Create directory for chromium base: ${CHROMIUM_GEN_DIR}/base"
        VERBATIM
)

add_custom_command(
        TARGET chromium_base_generate_headers
        POST_BUILD
        COMMAND mkdir -p "${CHROMIUM_GEN_DIR}/base/debug"
        WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
        COMMENT "Create directory for chromium base: ${CHROMIUM_GEN_DIR}/base/debug"
        VERBATIM
)

add_custom_command(
        TARGET chromium_base_generate_headers
        POST_BUILD
        COMMAND mkdir -p "${CHROMIUM_GEN_DIR}/base/allocator"
        WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
        COMMENT "Create directory for chromium base: ${CHROMIUM_GEN_DIR}/base/allocator"
        VERBATIM
)

add_custom_command(
        TARGET chromium_base_generate_headers
        POST_BUILD
        COMMAND "${CHROMIUM_SOURCE_DIR}/build/write_build_date_header.py" "${CHROMIUM_GEN_DIR}/base/generated_build_date.h" default
        WORKING_DIRECTORY "${CHROMIUM_BINARY_DIR}"
        COMMENT "Generate ${CHROMIUM_GEN_DIR}/base/generated_build_date.h by build/write_build_date_header.py"
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
add_dependencies(chromium_base chromium_base_generate_headers)
