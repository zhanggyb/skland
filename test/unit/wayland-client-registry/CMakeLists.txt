file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(wayland-client-registry ${sources} ${headers})
target_link_libraries(wayland-client-registry gtest skland)