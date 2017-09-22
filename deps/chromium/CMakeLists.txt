# deps/chromium

#cmake_minimum_required(VERSION 3.1)

#project(Chromium)

# The project version number.
#set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
#set(VERSION_MINOR 0 CACHE STRING "Project minor version number.")
#set(VERSION_PATCH 1 CACHE STRING "Project patch version number.")
#mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

#option(CHROMIUM_SOURCE_DIR "Directory of chromium source")
#option(CHROMIUM_BINARY_DIR "Directory to build chromium")
#option(CHROMIUM_GEN_DIR "Directory for generated files")

#if (NOT CHROMIUM_SOURCE_DIR)
#    set(CHROMIUM_SOURCE_DIR ${PROJECT_SOURCE_DIR})
#endif ()

#if (NOT CHROMIUM_BINARY_DIR)
#    set(CHROMIUM_BINARY_DIR ${PROJECT_BINARY_DIR})
#endif ()

#if (NOT CHROMIUM_GEN_DIR)
#    set(CHROMIUM_GEN_DIR "${CHROMIUM_BINARY_DIR}/gen")
#endif ()

add_subdirectory(base)
add_subdirectory(third_party)

add_custom_target(
        chromium
        COMMENT "Build chromium libraries"
)

add_dependencies(chromium chromium_base)
