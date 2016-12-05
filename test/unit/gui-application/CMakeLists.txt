file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(gui-application ${sources} ${headers})
target_link_libraries(gui-application gtest skland)