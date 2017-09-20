file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-posix-timer ${sources} ${headers})
target_link_libraries(core-posix-timer gtest skland)