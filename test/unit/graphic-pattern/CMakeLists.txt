file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(graphic-pattern ${sources} ${headers})
target_link_libraries(graphic-pattern gtest skland)