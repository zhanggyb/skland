file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(wayland-client-seat ${sources} ${headers})
target_link_libraries(wayland-client-seat gtest skland)
