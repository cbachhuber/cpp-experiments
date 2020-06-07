add_library(containers src/linked_list.cpp)

target_include_directories(containers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()
