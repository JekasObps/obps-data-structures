cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)

project(thread_pool VERSION 1.0)

add_library(thread_pool INTERFACE)

target_link_libraries(thread_pool INTERFACE spinlock)

target_include_directories(thread_pool
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests 
    ${CMAKE_CURRENT_BINARY_DIR}/tests
)