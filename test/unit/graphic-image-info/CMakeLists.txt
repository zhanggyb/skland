file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-image-info ${sources} ${headers})
target_link_libraries(graphic-image-info gtest skland)