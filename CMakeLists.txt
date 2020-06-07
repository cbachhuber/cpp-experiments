cmake_minimum_required(VERSION 3.10)
project(CppExperiments)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    add_subdirectory(extern/googletest)
    enable_testing()
    list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/extern)
    include (googletest)
endif()

add_executable(experiments main.cpp)

add_subdirectory(containers)
