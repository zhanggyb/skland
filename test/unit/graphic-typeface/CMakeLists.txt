file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-typeface ${sources} ${headers})
target_link_libraries(graphic-typeface gtest skland)