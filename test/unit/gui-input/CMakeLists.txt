file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-input ${sources} ${headers})
target_link_libraries(gui-input gtest skland)
