file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-trace ${sources} ${headers})
target_link_libraries(core-trace gtest skland)