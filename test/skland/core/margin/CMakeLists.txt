file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-margin ${sources} ${headers})
target_link_libraries(core-margin gtest skland)