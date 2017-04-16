file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-deque ${sources} ${headers})
target_link_libraries(core-deque gtest skland)