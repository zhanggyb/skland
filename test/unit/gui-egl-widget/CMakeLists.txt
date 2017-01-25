file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-egl-widget ${sources} ${headers})
target_link_libraries(gui-egl-widget gtest skland)