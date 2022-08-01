cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(timefreeze LANGUAGES C VERSION 0.0.1)

option(DEBUG "Enable debug output to stderr if DBG environment variable is set" ON)

option(COVERAGE "Enable capturing coverage information" OFF)
if(COVERAGE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR "COVERAGE can only be enabled in Debug mode")
    else()
        add_compile_options(-ftest-coverage -fprofile-arcs -g -p)
        add_link_options(-fprofile-arcs)
        message(STATUS "COVERAGE is enabled")
    endif()
endif()

add_compile_options(-Wall -Wextra)

add_library(timefreeze SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/libtimefreeze.c")
set_property(TARGET timefreeze PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET timefreeze PROPERTY C_STANDARD 99)
target_include_directories(timefreeze PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
if(DEBUG)
	target_compile_definitions(timefreeze PUBLIC TIMEFREEZE_DEBUG=1)
endif()
target_link_libraries(timefreeze dl)

add_executable(example "${CMAKE_CURRENT_SOURCE_DIR}/example/example.c")

find_package(Doxygen)
if(DOXYGEN_FOUND)
    message(STATUS "doxygen is available")
endif()
