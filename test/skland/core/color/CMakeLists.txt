file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-color ${sources} ${headers})
target_link_libraries(core-color gtest skland)