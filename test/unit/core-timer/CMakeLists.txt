file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-timer ${sources} ${headers})
target_link_libraries(core-timer gtest skland)