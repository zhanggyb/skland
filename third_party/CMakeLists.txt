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

# Set default options for OpenImageIO:
#set(OIIO_BUILD_TOOLS OFF CACHE BOOL "Do not build tools")
#set(OIIO_BUILD_TESTS OFF CACHE BOOL "Do not build tests")
#set(USE_QT OFF CACHE BOOL "Do not use Qt")
#set(USE_FIELD3D OFF CACHE BOOL "Do not use Field3D")
#set(USE_FFMPEG OFF CACHE BOOL "Do not use FFMPEG")
#set(USE_PYTHON OFF CACHE BOOL "Do not use python")
#set(USE_PYTHON3 OFF CACHE BOOL "Do not use python3")
#set(USE_NUKE OFF CACHE BOOL "Do not use nuke")
#set(USE_LIBRAW OFF CACHE BOOL "Do not use libraw")
#set(USE_DICOM OFF CACHE BOOL "Do not use dicom")
#set(BUILDSTATIC ON CACHE BOOL "Build static library")
#include_directories(BEFORE ${PROJECT_SOURCE_DIR}/third_party/oiio/src/include)   # Avoid using the old OIIO installed in system
#add_subdirectory(oiio)
