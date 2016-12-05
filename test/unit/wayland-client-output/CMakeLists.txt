file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(wayland-client-output ${sources} ${headers})
target_link_libraries(wayland-client-output gtest skland)
