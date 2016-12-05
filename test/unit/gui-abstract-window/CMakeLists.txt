file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-abstract-window ${sources} ${headers})
target_link_libraries(gui-abstract-window gtest skland)