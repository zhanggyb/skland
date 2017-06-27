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

file(GLOB numerical_headers "${PROJECT_SOURCE_DIR}/include/skland/numerical/*.hpp")
file(GLOB numerical_sources "numerical/*.cpp")
file(GLOB numerical_internal_headers "numerical/internal/*.hpp")
file(GLOB numerical_internal_sources "numerical/internal/*.cpp")

list(APPEND all_source_files
        ${numerical_headers}
        ${numerical_sources}
        ${numerical_internal_headers}
        ${numerical_internal_sources})

file(GLOB graphic_headers "${PROJECT_SOURCE_DIR}/include/skland/graphic/*.hpp")
file(GLOB graphic_sources "graphic/*.cpp")
file(GLOB graphic_internal_headers "graphic/internal/*.hpp")
file(GLOB graphic_internal_sources "graphic/internal/*.cpp")

list(APPEND all_source_files
        ${graphic_headers}
        ${graphic_sources}
        ${graphic_internal_headers}
        ${graphic_internal_sources})

#file(GLOB av_headers "${PROJECT_SOURCE_DIR}/include/skland/av/*.hpp")
#file(GLOB av_sources "av/*.cpp")
#file(GLOB av_internal_headers "av/internal/*.hpp")
#file(GLOB av_internal_sources "av/internal/*.cpp")
#
#list(APPEND all_source_files
#        ${av_headers}
#        ${av_sources}
#        ${av_internal_headers}
#        ${av_internal_sources})

if (LINUX)

    file(GLOB gui_headers "${PROJECT_SOURCE_DIR}/include/skland/gui/*.hpp")
    file(GLOB gui_sources "gui/*.cpp")
    file(GLOB gui_internal_headers "gui/internal/*.hpp")
    file(GLOB gui_internal_sources "gui/internal/*.cpp")

    file(GLOB stock_headers "${PROJECT_SOURCE_DIR}/include/skland/stock/*.hpp")
    file(GLOB stock_sources "stock/*.cpp")
    file(GLOB stock_internal_headers "stock/internal/*.hpp")
    file(GLOB stock_internal_sources "stock/internal/*.cpp")

    list(APPEND all_source_files
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

set(global_header "${PROJECT_SOURCE_DIR}/include/skland/config.hpp")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if (BUILD_SHARED_LIBRARY)
    add_library(skland SHARED ${global_header} ${all_source_files})
    set_target_properties(skland PROPERTIES VERSION 1 SOVERSION 1)
else ()
    add_library(skland ${global_header} ${all_source_files})
endif ()

target_include_directories(skland
        PRIVATE "${SKIA_PUBLIC_INCLUDES}"
        PRIVATE "${PROJECT_BINARY_DIR}/deps/wayland-protocols/unstable/xdg-shell"
        PRIVATE "${PROJECT_SOURCE_DIR}/include/skland")

if (LINUX)
    target_link_libraries(skland rt ${LIBS} wayland-cursor skia xdg-shell-unstable-v6-client)
else ()
    target_link_libraries(skland ${LIBS} skia)
endif ()

install(TARGETS skland DESTINATION lib)
