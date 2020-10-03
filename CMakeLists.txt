cmake_minimum_required(VERSION 3.10)
project(CppExperiments)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    add_subdirectory(extern/googletest)
    enable_testing()
    list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/extern)
    include (googletest)
endif()

add_executable(transform_performance transform_performance.cpp)
add_executable(std_bind_vs_lambda std_bind_vs_lambda.cpp)
add_executable(tmiv_encode_decode_architecture tmiv_encode_decode_architecture.cpp)

add_subdirectory(containers)
add_subdirectory(dojos)
