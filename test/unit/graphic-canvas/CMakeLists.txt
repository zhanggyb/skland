file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-canvas ${sources} ${headers})
target_link_libraries(graphic-canvas gtest skland)