file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-path ${sources} ${headers})
target_link_libraries(graphic-path gtest skland)