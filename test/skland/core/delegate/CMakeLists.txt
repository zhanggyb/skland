file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-delegate ${sources} ${headers})
target_link_libraries(core-delegate gtest skland)