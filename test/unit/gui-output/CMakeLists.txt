file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-output ${sources} ${headers})
target_link_libraries(gui-output gtest skland)
