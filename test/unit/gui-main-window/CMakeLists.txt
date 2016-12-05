file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-main-window ${sources} ${headers})
target_link_libraries(gui-main-window gtest skland)