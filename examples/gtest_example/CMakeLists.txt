cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(gtestexample LANGUAGES CXX)

find_package(GTest REQUIRED)

file(GLOB_RECURSE TestSources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(example-tests ${TestSources})
target_include_directories(example-tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(example-tests ${GTEST_LIBRARIES} pthread)
