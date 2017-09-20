file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-vectors ${sources} ${headers})
target_link_libraries(core-vectors gtest skland)