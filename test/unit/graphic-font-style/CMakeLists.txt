file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-font-style ${sources} ${headers})
target_link_libraries(graphic-font-style gtest skland)