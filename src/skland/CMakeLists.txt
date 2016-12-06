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

file(GLOB core_headers "${PROJECT_SOURCE_DIR}/include/skland/core/*.hpp")
file(GLOB core_detail_headers "${PROJECT_SOURCE_DIR}/include/skland/core/detail/*.hpp")
file(GLOB core_sources "core/*.cpp")

if (APPLE)
    list(REMOVE_ITEM core_headers "${PROJECT_SOURCE_DIR}/include/skland/core/timer.hpp")
    list(REMOVE_ITEM core_sources "${PROJECT_SOURCE_DIR}/src/skland/core/timer.cpp")
endif ()

list(APPEND all_source_files
        ${core_headers}
        ${core_detail_headers}
        ${core_sources})

file(GLOB graphic_headers "${PROJECT_SOURCE_DIR}/include/skland/graphic/*.hpp")
file(GLOB graphic_sources "graphic/*.cpp")

list(APPEND all_source_files
        ${graphic_headers}
        ${graphic_sources})

if (LINUX)

    file(GLOB wayland_protocol_headers "${PROJECT_SOURCE_DIR}/include/skland/wayland/protocol/*.h")
    file(GLOB wayland_protocol_sources "wayland/protocol/*.c")

    file(GLOB wayland_client_headers "${PROJECT_SOURCE_DIR}/include/skland/wayland/client/*.hpp")
    file(GLOB wayland_client_sources "wayland/client/*.cpp")

    file(GLOB gui_headers "${PROJECT_SOURCE_DIR}/include/skland/gui/*.hpp")
    file(GLOB gui_detail_headers "${PROJECT_SOURCE_DIR}/include/skland/gui/detail/*.hpp")
    file(GLOB gui_sources "gui/*.cpp")
    file(GLOB gui_detail_sources "gui/detail/*.cpp")

    file(GLOB stock_headers "${PROJECT_SOURCE_DIR}/include/skland/stock/*.hpp")
    file(GLOB stock_sources "stock/*.cpp")

    list(APPEND all_source_files
            ${wayland_protocol_headers}
            ${wayland_protocol_sources}
            ${wayland_client_headers}
            ${wayland_client_sources}
            ${gui_headers}
            ${gui_sources}
            ${gui_detail_headers}
            ${gui_detail_sources}
            ${stock_headers}
            ${stock_sources})

endif ()

set(global_header "${PROJECT_SOURCE_DIR}/include/skland/skland.hpp")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(skland ${global_header} ${all_source_files})

target_include_directories(skland PRIVATE "${SKIA_PUBLIC_INCLUDES}")

if (LINUX)
    target_link_libraries(skland rt ${LIBS} wayland-cursor skia)
else ()
    target_link_libraries(skland ${LIBS} skia)
endif ()

if (NOT DEFINED BUILD_STATIC_LIBRARY)
    set_target_properties(skland PROPERTIES VERSION 1 SOVERSION 1)
endif ()

install(TARGETS skland DESTINATION lib)
