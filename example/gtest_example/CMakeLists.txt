cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(gtestexample LANGUAGES CXX)

find_package(GTest REQUIRED)

file(GLOB_RECURSE TestSources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(tests ${TestSources})
target_include_directories(tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${GTEST_INCLUDE_DIRS}
)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
