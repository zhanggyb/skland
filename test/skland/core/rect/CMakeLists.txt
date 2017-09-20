file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-rect ${sources} ${headers})
target_link_libraries(core-rect gtest skland)