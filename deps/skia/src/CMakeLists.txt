
add_subdirectory(core)
add_subdirectory(effects)
add_subdirectory(fonts)
add_subdirectory(gpu)
add_subdirectory(image)
add_subdirectory(images)

set(SKIA_HEADERS ${SKIA_HEADERS} PARENT_SCOPE)
set(SKIA_SOURCES ${SKIA_SOURCES} PARENT_SCOPE)
