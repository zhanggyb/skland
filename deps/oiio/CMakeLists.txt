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

set(OIIO_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/oiio")
set(OIIO_BINARY_DIR "${PROJECT_BINARY_DIR}/deps/oiio    ")

set(CMAKE_MODULE_PATH
        "${OIIO_SOURCE_DIR}/src/cmake/modules"
        "${OIIO_SOURCE_DIR}/src/cmake")

include(oiio_macros)
include(platform)
include(compiler)   # All the C++ and compiler related options live here

option(USE_OPENGL "Include OpenGL support" ON)
set(JPEG_PATH "" CACHE STRING "Custom JPEG path")
option(USE_JPEGTURBO "Use JPEG-Turbo if found" ON)
set(JPEGTURBO_PATH "" CACHE STRING "Custom JPEG-Turbo path")
option(USE_OPENJPEG "Use OpenJpeg if found" ON)
option(USE_OCIO "Use OpenColorIO for color management if found" ON)
option(USE_OPENCV "Use OpenCV if found" ON)
option(USE_OPENSSL "Use OpenSSL if found (for faster SHA-1)")
option(USE_FREETYPE "Use Freetype if found" ON)
option(USE_GIF "Use GIF if found" ON)
option(USE_PTEX "Use PTex if found" ON)
option(OIIO_THREAD_ALLOW_DCLP "OIIO threads may use DCLP for speed" ON)
set(USE_EXTERNAL_PUGIXML OFF CACHE BOOL
        "Use an externally built shared library version of the pugixml library")
set(PUGIXML_HOME "" CACHE STRING "Hint about where to find external PugiXML library")

if (NOT OIIO_THREAD_ALLOW_DCLP)
    add_definitions("-DOIIO_THREAD_ALLOW_DCLP=0")
endif ()

# Set the default namespace
set(PROJ_NAMESPACE "${${PROJ_NAME}_NAMESPACE}")
set(PROJ_NAMESPACE_V "${PROJ_NAMESPACE}_v${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")
message(STATUS "Setting Namespace to: ${PROJ_NAMESPACE_V}")


include(externalpackages)

include_directories(
        BEFORE
        "${CMAKE_SOURCE_DIR}/src/include"
        "${CMAKE_BINARY_DIR}/include"
)

# Add IO plugin directories -- if we are embedding plugins, we need to visit
# these directories BEFORE the OpenImageIO target is established (in
# src/libOpenImageIO). For each plugin, we append to the lists of source
# files, format libs, include directories, and definitions, all of which
# will be used by src/libOpenImageIO.
set(libOpenImageIO_srcs "")
set(format_plugin_libs "")
set(format_plugin_include_dirs "")
set(format_plugin_definitions "")
file(GLOB all_format_plugin_dirs "${OIIO_SOURCE_DIR}/src/*.imageio")
if ("${OIIO_SITE}" STREQUAL "SPI")
    # SPI only -- because of a workaround for a very weird linkage issue
    # specific to our system, we need to be sure libtiff is referenced first.
    file(GLOB tiff_format_plugin_dir "${OIIO_SOURCE_DIR}/src/tiff.imageio")
    list(REMOVE_ITEM all_format_plugin_dirs ${tiff_format_plugin_dir})
    list(INSERT all_format_plugin_dirs 0 ${tiff_format_plugin_dir})
endif ()
if (EMBEDPLUGINS AND NOT BUILD_OIIOUTIL_ONLY)
    foreach (plugin_dir ${all_format_plugin_dirs})
        add_subdirectory(${plugin_dir})
    endforeach ()
endif ()

add_subdirectory(src/libOpenImageIO)

# Add IO plugin directories -- if we are not embedding plugins, we need to
# do it AFTER the OpenImageIO target is established (in src/libOpenImageIO),
# since each plugin needs libOpenImageIO to be a dependency.
if (NOT EMBEDPLUGINS AND NOT BUILD_OIIOUTIL_ONLY)
    foreach (plugin_dir ${all_format_plugin_dirs})
        add_subdirectory(${plugin_dir})
    endforeach ()
endif ()

add_subdirectory(${OIIO_SOURCE_DIR}/src/include)
add_subdirectory(${OIIO_SOURCE_DIR}/src/fonts)
