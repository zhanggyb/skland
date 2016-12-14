file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-font ${sources} ${headers})
target_link_libraries(graphic-font gtest skland)