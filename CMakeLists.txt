cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

set(MAJOR_VERSION 0)
project(timefreeze LANGUAGES C VERSION "${MAJOR_VERSION}.0.1")

option(DEBUG "Enable debug output to stderr if DBG environment variable is set" ON)

add_compile_options(-Wall -Wextra)

add_library(timefreeze SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/libtimefreeze.c")
target_include_directories(timefreeze PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
if(DEBUG)
    target_compile_definitions(timefreeze PRIVATE TIMEFREEZE_DEBUG=1)
endif()
target_link_libraries(timefreeze dl)
set_target_properties(timefreeze PROPERTIES
    SOVERSION "${MAJOR_VERSION}"
    POSITION_INDEPENDENT_CODE 1
    C_STANDARD 99)

option(COVERAGE "Enable capturing coverage information" OFF)
if(COVERAGE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR "COVERAGE can only be enabled in Debug mode")
    else()
        target_compile_options(timefreeze PRIVATE -ftest-coverage -fprofile-arcs -g -p)
        set_target_properties(timefreeze PROPERTIES LINK_FLAGS "-fprofile-arcs")
        message(STATUS "COVERAGE is enabled")
    endif()
endif()

add_executable(example "${CMAKE_CURRENT_SOURCE_DIR}/example/example.c")
