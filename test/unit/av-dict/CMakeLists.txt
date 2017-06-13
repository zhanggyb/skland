file(GLOB sources "*.cpp")
file(GLOB headers "*.hpp")

add_executable(av-dict ${sources} ${headers})
target_link_libraries(av-dict gtest skland)