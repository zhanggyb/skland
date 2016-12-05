file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(wayland-client-display ${sources} ${headers})
target_link_libraries(wayland-client-display gtest skland)