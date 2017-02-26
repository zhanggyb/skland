# To first approximation, the Skia library comprises all .cpp files under src/.
file(GLOB_RECURSE srcs ${SKIA_SOURCE_DIR}/src/*.cpp)

function(find_include_dirs out)
    file(GLOB_RECURSE headers ${ARGN})
    foreach (path ${headers})
        get_filename_component(dir ${path} PATH)
        list(APPEND include_dirs ${dir})
    endforeach ()
    list(REMOVE_DUPLICATES include_dirs)
    set(${out} ${include_dirs} PARENT_SCOPE)
endfunction()

# We need src/ directories and include/private on our path when building Skia.
# Users should be able to use Skia with only include/ directories that are not include/private.
find_include_dirs(private_includes ${SKIA_SOURCE_DIR}/src/*.h ${SKIA_SOURCE_DIR}/include/private/*.h)
find_include_dirs(public_includes ${SKIA_SOURCE_DIR}/include/*.h)
list(REMOVE_ITEM public_includes ${private_includes})  # Easiest way to exclude private.
file(GLOB default_include_config "${SKIA_SOURCE_DIR}/include/config")
list(REMOVE_ITEM public_includes ${default_include_config})
set(userconfig_directory ${CMAKE_BINARY_DIR}/include)
list(APPEND public_includes ${userconfig_directory})

# These guys are third_party but provided by a Skia checkout.
list(APPEND srcs ${SKIA_SOURCE_DIR}/third_party/etc1/etc1.cpp)  # ${SKIA_SOURCE_DIR}/third_party/ktx/ktx.cpp)
list(APPEND private_includes ${SKIA_SOURCE_DIR}/third_party/etc1)   # ${SKIA_SOURCE_DIR}/third_party/ktx)

list(APPEND srcs ${SKIA_SOURCE_DIR}/third_party/gif/SkGifImageReader.cpp)
list(APPEND private_includes ${SKIA_SOURCE_DIR}/third_party/gif)

list(APPEND private_includes ${SKIA_SOURCE_DIR}/third_party/libpng)
file(GLOB libpng_srcs ${SKIA_SOURCE_DIR}/third_party/libpng/*.c)
foreach (src ${libpng_srcs})
    list(APPEND srcs ${src})
endforeach ()


function(remove_srcs)
    file(GLOB_RECURSE to_remove ${ARGN})
    list(REMOVE_ITEM srcs ${to_remove})
    set(srcs ${srcs} PARENT_SCOPE)
endfunction()

# This file is empty and is only used to trick GYP.
remove_srcs(${SKIA_SOURCE_DIR}/src/core/SkForceCPlusPlusLinking.cpp)
# Chrome only?
remove_srcs(${SKIA_SOURCE_DIR}/src/ports/SkFontConfigInterface.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkFontConfigInterface_direct.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkFontConfigInterface_direct_factory.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkFontConfigInterface_direct_google3.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkFontConfigInterface_direct_google3_factory.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_FontConfigInterface.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_FontConfigInterface_factory.cpp)
# Alternative font managers.
remove_srcs(${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_custom*.cpp)
# skslc main()
remove_srcs(${SKIA_SOURCE_DIR}/src/sksl/SkSLMain.cpp)
# remove the special splicer source, TODO: config cmake to detect clang and SSE
remove_srcs(${SKIA_SOURCE_DIR}/src/splicer/SkSplicer_stages.cpp)
# remove_srcs(${SKIA_SOURCE_DIR}/src/splicer/SkSplicer_stages_lowp.cpp)


# Skia sure ships a lot of code no one uses.
remove_srcs(${SKIA_SOURCE_DIR}/src/svg/*
        ${SKIA_SOURCE_DIR}/src/views/*
        ${SKIA_SOURCE_DIR}/src/xml/*
        ${SKIA_SOURCE_DIR}/src/jumper/*)
foreach (include svg views gpu/vk)
    file(GLOB globed_include ${SKIA_SOURCE_DIR}/include/${include})
    list(REMOVE_ITEM public_includes ${globed_include})
endforeach ()

# Remove OS-specific source files.
if (NOT UNIX)
    remove_srcs(${SKIA_SOURCE_DIR}/src/ports/*_posix.cpp
            ${SKIA_SOURCE_DIR}/src/ports/SkTLS_pthread.cpp
            ${SKIA_SOURCE_DIR}/src/ports/SkTime_Unix.cpp
            ${SKIA_SOURCE_DIR}/src/utils/SkThreadUtils_pthread.cpp)
endif ()
if (APPLE OR NOT UNIX)
    remove_srcs(${SKIA_SOURCE_DIR}/src/gpu/gl/glx/*
            ${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_fontconfig*.cpp
            ${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_android*.cpp
            ${SKIA_SOURCE_DIR}/src/*FreeType*)
endif ()

# Remove processor-specific source files.
if (NOT CMAKE_SYSTEM_PROCESSOR STREQUAL ARM)
    remove_srcs(${SKIA_SOURCE_DIR}/src/*arm* ${SKIA_SOURCE_DIR}/src/*ARM* ${SKIA_SOURCE_DIR}/src/*neon* ${SKIA_SOURCE_DIR}/src/*NEON*)
endif ()
#if (NOT CMAKE_SYSTEM_PROCESSOR STREQUAL MIPS)
#    remove_srcs(${SKIA_SOURCE_DIR}/src/*mips* ${SKIA_SOURCE_DIR}/src/*MIPS*)
#endif ()

# Make our ports choices.
remove_srcs(
        ${SKIA_SOURCE_DIR}/src/*moz*   # We're probably not Mozilla.
        ${SKIA_SOURCE_DIR}/src/gpu/GrContextFactory.cpp                  # For internal testing only.
        ${SKIA_SOURCE_DIR}/src/gpu/gl/GrGLCreateNativeInterface_none.cpp
        ${SKIA_SOURCE_DIR}/src/gpu/gl/GrGLDefaultInterface_none.cpp
        ${SKIA_SOURCE_DIR}/src/gpu/gl/command_buffer/*
        ${SKIA_SOURCE_DIR}/src/gpu/gl/egl/*
        ${SKIA_SOURCE_DIR}/src/gpu/gl/glfw/*
        ${SKIA_SOURCE_DIR}/src/gpu/gl/iOS/*
        ${SKIA_SOURCE_DIR}/src/gpu/vk/*
        ${SKIA_SOURCE_DIR}/src/opts/SkBitmapProcState_opts_none.cpp
        ${SKIA_SOURCE_DIR}/src/opts/SkBlitMask_opts_none.cpp
        ${SKIA_SOURCE_DIR}/src/opts/SkBlitRow_opts_none.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_empty_factory.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkGlobalInitialization_chromium.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkImageEncoder_none.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkImageGenerator_none.cpp
        ${SKIA_SOURCE_DIR}/src/ports/SkTLS_none.cpp)

if (NOT APPLE)
    remove_srcs(${SKIA_SOURCE_DIR}/src/ports/SkImageGeneratorCG.cpp)
endif ()

if (NOT WIN32)
    remove_srcs(${SKIA_SOURCE_DIR}/src/ports/SkImageGeneratorWIC.cpp)
endif ()

if (WIN32)
    if (SKIA_GDI)
        remove_srcs(${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_win_dw_factory.cpp)
    else ()
        remove_srcs(${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_win_gdi_factory.cpp)
    endif ()
endif ()

# Certain files must be compiled with support for SSSE3, SSE4.1, AVX, or AVX2 intrinsics.
file(GLOB_RECURSE ssse3_srcs ${SKIA_SOURCE_DIR}/src/*ssse3*.cpp ${SKIA_SOURCE_DIR}/src/*SSSE3*.cpp)
file(GLOB_RECURSE sse41_srcs ${SKIA_SOURCE_DIR}/src/*sse41*.cpp ${SKIA_SOURCE_DIR}/src/*SSE41*.cpp)
file(GLOB_RECURSE sse42_srcs ${SKIA_SOURCE_DIR}/src/*sse42*.cpp ${SKIA_SOURCE_DIR}/src/*SSE42*.cpp)
file(GLOB_RECURSE avx_srcs ${SKIA_SOURCE_DIR}/src/*_avx.cpp)
file(GLOB_RECURSE hsw_srcs ${SKIA_SOURCE_DIR}/src/*_hsw.cpp)
if (NOT WIN32)
    set_source_files_properties(${ssse3_srcs} PROPERTIES COMPILE_FLAGS -mssse3)
    set_source_files_properties(${sse41_srcs} PROPERTIES COMPILE_FLAGS -msse4.1)
    set_source_files_properties(${sse42_srcs} PROPERTIES COMPILE_FLAGS -msse4.2)
    set_source_files_properties(${avx_srcs} PROPERTIES COMPILE_FLAGS -mavx)
    set_source_files_properties(${hsw_srcs} PROPERTIES COMPILE_FLAGS
            -mavx2 -mbmi -mbmi2 -mf16c -mfma)
endif ()

# Detect our optional dependencies.
# If we can't find them, don't build the parts of Skia that use them.
find_package(EXPAT)
#find_package(Lua)  // Note: there's a code bug in current Skia master, to make it work, add '#include "SkVertices.h"' in SkLuaCanvas.h
find_package(ZLIB)

# For libraries that don't have find_package() scripts, we do it ourselves:
find_path(WEBP_INCLUDE_DIRS "webp/decode.h")
find_library(WEBP_LIBRARIES "webp")
find_library(WEBPDEMUX_LIBRARY "webpdemux")

find_path(OSMESA_INCLUDE_DIRS "GL/osmesa.h")
find_library(OSMESA_LIBRARIES "OSMesa")

if (UNIX AND NOT APPLE)
    find_package(Freetype)
    find_package(GIF)

    find_path(FONTCONFIG_INCLUDE_DIRS "fontconfig/fontconfig.h")
    find_library(FONTCONFIG_LIBRARIES fontconfig)

    # We require libjpeg-turbo >= 1.5.0.
    find_package(PkgConfig)
    pkg_check_modules(JPEG_TURBO libjpeg>=1.5.0)
endif ()

# Do not compile SkRawCodec.
remove_srcs(${SKIA_SOURCE_DIR}/src/codec/*Raw*.cpp)

# TODO: macro away this if (found) ... else() ... endif() stuff.

if (EXPAT_FOUND)
    list(APPEND private_includes ${EXPAT_INCLUDE_DIRS})
    list(APPEND libs ${EXPAT_LIBRARIES})
else ()
    remove_srcs(${SKIA_SOURCE_DIR}/src/ports/SkFontMgr_android_parser.cpp)
endif ()

if (GIF_FOUND)
    list(APPEND private_includes ${GIF_INCLUDE_DIRS})
    list(APPEND libs ${GIF_LIBRARIES})
    add_definitions(-DSK_HAS_GIF_LIBRARY)
else ()
    # No such files
    # remove_srcs(${SKIA_SOURCE_DIR}/src/images/*GIF*)
    remove_srcs(${SKIA_SOURCE_DIR}/src/codec/*Gif*)
endif ()

if (JPEG_TURBO_FOUND)
    list(APPEND private_includes ${JPEG_TURBO_INCLUDE_DIRS})
    list(APPEND libs ${JPEG_TURBO_LIBRARIES})
    add_definitions(-DSK_HAS_JPEG_LIBRARY)
else ()
    remove_srcs(${SKIA_SOURCE_DIR}/src/images/*JPEG*)
    remove_srcs(${SKIA_SOURCE_DIR}/src/codec/*Jpeg*)
endif ()

if (LUA_FOUND)
    list(APPEND private_includes ${LUA_INCLUDE_DIR})
    list(APPEND libs ${LUA_LIBRARIES})
else ()
    remove_srcs(${SKIA_SOURCE_DIR}/src/utils/*Lua*)
endif ()

# PNG
add_definitions(-DSK_HAS_PNG_LIBRARY)
add_definitions(-DPNG_ARM_NEON_OPT=0)

if (ZLIB_FOUND)
    list(APPEND private_includes ${ZLIB_INCLUDE_DIRS})
    list(APPEND libs ${ZLIB_LIBRARIES})
    remove_srcs(${SKIA_SOURCE_DIR}/src/pdf/SkDocument_PDF_None.cpp)
else ()
    remove_srcs(${SKIA_SOURCE_DIR}/src/pdf/*.cpp)
    set(srcs ${srcs} ${SKIA_SOURCE_DIR}/src/pdf/SkDocument_PDF_None.cpp)
endif ()

if (WEBP_INCLUDE_DIRS AND WEBP_LIBRARIES)
    list(APPEND private_includes ${WEBP_INCLUDE_DIRS})
    list(APPEND libs ${WEBP_LIBRARIES})
    if (WEBPDEMUX_LIBRARY)
        list(APPEND libs ${WEBPDEMUX_LIBRARY})
    endif ()
    add_definitions(-DSK_HAS_WEBP_LIBRARY)
else ()
    remove_srcs(${SKIA_SOURCE_DIR}/src/images/*WEBP*)
    remove_srcs(${SKIA_SOURCE_DIR}/src/codec/*Webp*)
endif ()

if (FREETYPE_FOUND)
    list(APPEND private_includes ${FREETYPE_INCLUDE_DIRS})
    list(APPEND libs ${FREETYPE_LIBRARIES})
endif ()

if (FONTCONFIG_INCLUDE_DIRS AND FONTCONFIG_LIBRARIES)
    list(APPEND private_includes ${FONTCONFIG_INCLUDE_DIRS})
    list(APPEND libs ${FONTCONFIG_LIBRARIES})
endif ()

if (APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices REQUIRED)
    list(APPEND libs ${APPLICATION_SERVICES_FRAMEWORK})
endif ()

if (OSMESA_LIBRARIES AND OSMESA_INCLUDE_DIRS)
    list(APPEND libs ${OSMESA_LIBRARIES})
    list(APPEND private_includes ${OSMESA_INCLUDE_DIRS})
    list(APPEND public_defines "-DSK_MESA=1")
    set(SK_MESA 1)
else ()
    remove_srcs(${SKIA_SOURCE_DIR}/src/gpu/gl/mesa/*)
endif ()

if (WIN32)
    list(APPEND libs FontSub.lib Usp10.lib)
else ()
    list(APPEND libs pthread)
endif ()

find_package(OpenGL REQUIRED)
list(APPEND libs ${OPENGL_LIBRARIES})

# This is our main output, libskia.{a,so,dll,dylib,etc...}
# You can control whether this is static or shared with BUILD_SHARED_LIBS.
add_library(skia ${srcs})

target_compile_definitions(skia
        PUBLIC ${public_defines}
        PRIVATE -DSKIA_DLL -DSKIA_IMPLEMENTATION=1)

target_include_directories(skia
        PUBLIC ${public_includes}
        PRIVATE ${private_includes})

target_link_libraries(skia
        PUBLIC
        PRIVATE ${libs})

if (MSVC)
    string(REGEX REPLACE " /W3 " " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(cc_flags "/w /GR-")
else ()
    set(cc_flags "-w -fno-rtti -fno-exceptions")
endif ()

# TODO: add macro to check SSE feature
# See SK_CPU_SSE_LEVEL in ${SKIA_SOURCE_DIR}/include/core/SkPreConfig.h
add_definitions(-DSK_CPU_SSE_LEVEL=42)
set(cc_flags "${cc_flags} -msse4.2")
#set(cc_flags "${cc_flags} -mfma -msse -msse2 -msse3 -msse4.1 -msse4.2 -mavx -mavx2")

# Global definitions:
add_definitions(-DSK_IGNORE_ETC1_SUPPORT)   # avoid 'ktx.h' error

set_target_properties(skia PROPERTIES
        COMPILE_FLAGS ${cc_flags})
if (BUILD_SHARED_LIBS)
    set_target_properties(skia PROPERTIES
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN true)
endif ()

# Experimental C API install:
file(GLOB c_headers "${SKIA_SOURCE_DIR}/include/c/*.h")
install(FILES ${c_headers} DESTINATION include)
install(TARGETS skia DESTINATION lib)

# SkUserConfig.h
if (CMAKE_BUILD_TYPE STREQUAL Release)
    set(SK_RELEASE 1)
else ()
    set(SK_RELEASE 0)
endif ()
if (UNIX AND NOT APPLE)
    set(SK_SAMPLES_FOR_X 1)
else ()
    set(SK_SAMPLES_FOR_X 0)
endif ()
configure_file("SkUserConfig.h.in" "${userconfig_directory}/SkUserConfig.h")

# skia_link_arguments.txt
set(link_arguments ${CMAKE_BINARY_DIR}/skia_link_arguments.txt)
if (BUILD_SHARED_LIBS)
    file(WRITE ${link_arguments} "-L${CMAKE_BINARY_DIR}\n")
    file(APPEND ${link_arguments} "-lskia\n")
    file(APPEND ${link_arguments} "-Wl,-rpath,${CMAKE_BINARY_DIR}\n")
else ()
    file(WRITE ${link_arguments} "${CMAKE_BINARY_DIR}/libskia.a\n")
    foreach (lib ${libs})
        if (EXISTS ${lib})
            get_filename_component(lib_path ${lib} ABSOLUTE)
            file(APPEND ${link_arguments} "${lib_path}\n")
        else ()
            file(APPEND ${link_arguments} "-l${lib}\n")
        endif ()
    endforeach ()
endif ()

# skia_compile_arguments.txt
set(compile_arguments ${CMAKE_BINARY_DIR}/skia_compile_arguments.txt)
file(WRITE ${compile_arguments} "--std=c++11\n")
foreach (include ${public_includes})
    get_filename_component(abs_include ${include} ABSOLUTE)
    file(APPEND ${compile_arguments} "-I${abs_include}\n")
endforeach ()

# cmake .
# cmake --build . --target skia
# c++ -c @skia_compile_arguments.txt example.cpp
# c++ example.o @skia_link_arguments.txt

# skia.h
set(bad_files GrGLConfig_chrome.h)
# make `c++ @skia_compile_arguments.txt include/skia.h` work.
set(skia_h_path ${userconfig_directory}/skia.h)
file(WRITE ${skia_h_path} "// skia.h generated by CMake.\n")
file(APPEND ${skia_h_path} "#ifndef skia_DEFINED\n")
file(APPEND ${skia_h_path} "#define skia_DEFINED\n")
foreach (include ${public_includes})
    if (NOT include STREQUAL userconfig_directory)
        file(APPEND ${skia_h_path} "\n")
        file(GLOB all_public_headers ${include}/*.h)
        foreach (public_header ${all_public_headers})
            get_filename_component(filename_component ${public_header} NAME)
            if (NOT ";${bad_files};" MATCHES ";${filename_component};")
                file(APPEND ${skia_h_path} "#include \"${filename_component}\"\n")
            endif ()
        endforeach ()
    endif ()
endforeach ()
file(APPEND ${skia_h_path} "\n#endif  // skia_DEFINED\n")

# Now build a simple example app that uses Skia via libskia.so.
#add_executable(example example.cpp)
#target_link_libraries(example skia ${OPENGL_LIBRARIES})
