file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(core-object ${sources} ${headers})
target_link_libraries(core-object gtest skland)