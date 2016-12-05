file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-abstract-widget ${sources} ${headers})
target_link_libraries(gui-abstract-widget gtest skland)