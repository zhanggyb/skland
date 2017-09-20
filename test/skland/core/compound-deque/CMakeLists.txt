file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-compound-deque ${sources} ${headers})
target_link_libraries(core-compound-deque gtest skland)