# deps/chromium

set(CHROMIUM_SOURCE_DIR "${PROJECT_SOURCE_DIR}/third_party/chromium/src")
set(CHROMIUM_BINARY_DIR "${PROJECT_BINARY_DIR}/deps/chromium")
set(CHROMIUM_GEN_DIR "${CHROMIUM_BINARY_DIR}/gen")

include_directories(${CHROMIUM_SOURCE_DIR})
include_directories(${CHROMIUM_GEN_DIR})

add_subdirectory(base)
