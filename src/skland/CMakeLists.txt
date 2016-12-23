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
file(GLOB core_internal_headers "${PROJECT_SOURCE_DIR}/include/skland/core/internal/*.hpp")
file(GLOB core_sources "core/*.cpp")

if (APPLE)
    list(REMOVE_ITEM core_headers "${PROJECT_SOURCE_DIR}/include/skland/core/posix-timer.hpp")
    list(REMOVE_ITEM core_sources "${PROJECT_SOURCE_DIR}/src/skland/core/posix-timer.cpp")
endif ()

list(APPEND all_source_files
        ${core_headers}
        ${core_internal_headers}
        ${core_sources})

file(GLOB graphic_headers "${PROJECT_SOURCE_DIR}/include/skland/graphic/*.hpp")
file(GLOB graphic_sources "graphic/*.cpp")
file(GLOB graphic_internal_headers "graphic/*.hpp")
file(GLOB graphic_internal_sources "graphic/*.cpp")

list(APPEND all_source_files
        ${graphic_headers}
        ${graphic_sources}
        ${graphic_internal_headers}
        ${graphic_internal_sources})

if (LINUX)

    file(GLOB wayland_client_headers "${PROJECT_SOURCE_DIR}/include/skland/wayland/client/*.hpp")
    file(GLOB wayland_client_sources "wayland/client/*.cpp")
    file(GLOB wayland_client_internal_headers "wayland/client/internal/*.hpp")
    file(GLOB wayland_client_internal_sources "wayland/client/internal/*.cpp")

    file(GLOB egl_headers "${PROJECT_SOURCE_DIR}/include/skland/egl/*.hpp")
    file(GLOB egl_sources "egl/*.cpp")

    file(GLOB gui_headers "${PROJECT_SOURCE_DIR}/include/skland/gui/*.hpp")
    file(GLOB gui_internal_headers "${PROJECT_SOURCE_DIR}/include/skland/gui/internal/*.hpp")
    file(GLOB gui_sources "gui/*.cpp")
    file(GLOB gui_internal_sources "gui/internal/*.cpp")

    file(GLOB stock_headers "${PROJECT_SOURCE_DIR}/include/skland/stock/*.hpp")
    file(GLOB stock_sources "stock/*.cpp")
    file(GLOB stock_internal_headers "stock/internal/*.hpp")
    file(GLOB stock_internal_sources "stock/internal/*.cpp")

    list(APPEND all_source_files
            ${wayland_client_headers}
            ${wayland_client_sources}
            ${wayland_client_internal_headers}
            ${wayland_client_internal_sources}
            ${egl_headers}
            ${egl_sources}
            ${gui_headers}
            ${gui_sources}
            ${gui_internal_headers}
            ${gui_internal_sources}
            ${stock_headers}
            ${stock_sources}
            ${stock_internal_headers}
            ${stock_internal_sources})

endif ()

set(global_header "${PROJECT_SOURCE_DIR}/include/skland/skland.hpp")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(skland ${global_header} ${all_source_files})

target_include_directories(skland
        PRIVATE "${SKIA_PUBLIC_INCLUDES}"
        PRIVATE "${PROJECT_BINARY_DIR}/lib/wayland-protocols/unstable/xdg-shell")

if (LINUX)
    target_link_libraries(skland rt ${LIBS} wayland-cursor skia xdg-shell-unstable-v6-client)
else ()
    target_link_libraries(skland ${LIBS} skia)
endif ()

if (NOT DEFINED BUILD_STATIC_LIBRARY)
    set_target_properties(skland PROPERTIES VERSION 1 SOVERSION 1)
endif ()

install(TARGETS skland DESTINATION lib)
