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

cmake_minimum_required(VERSION 3.1)

project(skland)

# The project version number.
set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 0 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 1 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules/")

# You may need to set environment variable CMAKE_PREFIX_PATH, see http://doc.qt.io/qt-5/cmake-manual.html
option(BUILD_UNIT_TEST "Build unit test code" OFF)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(APPLE TRUE)
    set(UNIX TRUE)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX TRUE)
    set(UNIX TRUE)
else ()
    message(FATAL_ERROR "Unsupported system!")
endif ()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()
set(CMAKE_CXX_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
else ()
    set(CMAKE_CXX_FLAGS "-O3 -std=c++11")
endif ()

set(SKIA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/third_party/skia)
list(APPEND SKIA_PUBLIC_INCLUDES
        "${SKIA_SOURCE_DIR}/include/android"
        "${SKIA_SOURCE_DIR}/include/c"
        "${SKIA_SOURCE_DIR}/include/codec"
        "${SKIA_SOURCE_DIR}/include/core"
        "${SKIA_SOURCE_DIR}/include/effects"
        "${SKIA_SOURCE_DIR}/include/gpu"
        "${SKIA_SOURCE_DIR}/include/gpu/effects"
        "${SKIA_SOURCE_DIR}/include/gpu/gl"
        "${SKIA_SOURCE_DIR}/include/images"
        "${SKIA_SOURCE_DIR}/include/pathops"
        "${SKIA_SOURCE_DIR}/include/ports"
        "${SKIA_SOURCE_DIR}/include/utils"
        "${SKIA_SOURCE_DIR}/include/private")

add_subdirectory(lib)

if (LINUX)

    find_package(EGL)
    if (EGL_FOUND)
        # set(HAVE_EGL 1)
        include_directories(${EGL_INCLUDE_DIRS})
        set(LIBS ${LIBS} ${EGL_LIBRARIES})
    endif ()

    find_package(Wayland REQUIRED)
    if (WAYLAND_FOUND)
        include_directories(${WAYLAND_CLIENT_INCLUDE_DIR} ${WAYLAND_EGL_INCLUDE_DIR})
        set(LIBS ${LIBS} ${WAYLAND_CLIENT_LIBRARIES} ${WAYLAND_EGL_LIBRARIES})
    endif ()

    #add_definitions(-D__UNIX__)

endif ()

find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if (BUILD_DOCUMENTATION)
    if (NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif ()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    set(doxy_main_page ${CMAKE_CURRENT_SOURCE_DIR}/doc/mainpage.dox)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
endif ()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)

if (LINUX)
    add_subdirectory(examples)
endif ()

if (BUILD_UNIT_TEST)
    add_subdirectory(test)
endif ()

# ----------------------------------------------------------------------------
# Generate config.hpp
# ----------------------------------------------------------------------------
configure_file("${PROJECT_SOURCE_DIR}/include/skland/config.hpp.in"
        "${PROJECT_SOURCE_DIR}/include/skland/config.hpp" @ONLY)

# ----------------------------------------------------------------------------
# Installation:
# ----------------------------------------------------------------------------

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/skland" DESTINATION include)
# TODO: exclude "config.hpp.in"


# ----------------------------------------------------------------------------
# Summary:
# ----------------------------------------------------------------------------

message(STATUS "")
if (APPLE)
    message(STATUS "  Only \'core\' and \'graphic\' modules will be built on macOS for unit test.")
else()
message(STATUS "  Install path: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Use Wayland client library: ${WAYLAND_CLIENT_LIBRARIES}")
message(STATUS "  Use Cairo library: ${CAIRO_LIBRARIES}")
if (EGL_FOUND)
message(STATUS "  Use EGL library: ${EGL_LIBRARIES}")
endif ()
endif()
message(STATUS "")
