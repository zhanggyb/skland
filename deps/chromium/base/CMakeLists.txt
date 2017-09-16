# deps/chromium/base

set(CHROMIUM_BASE_DIR "${CHROMIUM_SOURCE_DIR}/base")

file(GLOB CHROMIUM_BASE_HEADERS "${CHROMIUM_BASE_DIR}/*.h")
file(GLOB CHROMIUM_BASE_SOURCES "${CHROMIUM_BASE_DIR}/*.cc")

file_list_remove(CHROMIUM_BASE_SOURCES GLOB "${CHROMIUM_BASE_DIR}/*_unittest.cc")

add_subdirectory(allocator)
add_subdirectory(containers)
add_subdirectory(debug)
add_subdirectory(files)
add_subdirectory(i18n)
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

add_library(chromium_base ${CHROMIUM_BASE_HEADERS} ${CHROMIUM_BASE_SOURCES})
#target_link_libraries(chromium_base
#        PRIVATE chromium_base_allocator
#        # PRIVATE chromium_base_containers  # header file only
#        PRIVATE chromium_base_time
#        )