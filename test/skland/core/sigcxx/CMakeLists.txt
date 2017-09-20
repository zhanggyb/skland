file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-sigcxx ${sources} ${headers})
target_link_libraries(core-sigcxx gtest skland)