file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-paint ${sources} ${headers})
target_link_libraries(graphic-paint gtest skland)